package envoy

import (
	"sync"
	"unsafe"
)

var memManager memoryManager

type (
	// memoryManager manages the heap allocated objects.
	// It is used to pin the objects to the heap to avoid them being garbage collected by the Go runtime.
	//
	// TODO: shard the linked lists to reduce contention. E.g. we can use the thread id to shard the linked lists.
	memoryManager struct {
		// httpFilters holds a linked lists of HttpFilter.
		httpFilters      *pinedHttpFilter
		httpFiltersMutex sync.Mutex

		// httpFilterInstances holds a linked lists of HttpFilterInstance.
		httpFilterInstances      *pinedHttpFilterInstance
		httpFilterInstancesMutex sync.Mutex
	}

	// pinedHttpFilter holds a pinned HttpFilter managed by the memory manager.
	pinedHttpFilter = linkedList[HttpFilter]

	// pinedHttpFilterInstance holds a pinned HttpFilterInstance managed by the memory manager.
	pinedHttpFilterInstance = linkedList[HttpFilterInstance]

	linkedList[T any] struct {
		obj        T
		next, prev *linkedList[T]
	}
)

// pinHttpFilter pins the HttpFilter to the memory manager.
func (m *memoryManager) pinHttpFilter(filter HttpFilter) *pinedHttpFilter {
	m.httpFiltersMutex.Lock()
	defer m.httpFiltersMutex.Unlock()

	item := &pinedHttpFilter{obj: filter, next: m.httpFilters, prev: nil}
	if m.httpFilters != nil {
		m.httpFilters.prev = item
	}
	m.httpFilters = item
	return item
}

func (m *memoryManager) unpinHttpFilter(filter *pinedHttpFilter) {
	m.httpFiltersMutex.Lock()
	defer m.httpFiltersMutex.Unlock()
	if filter.prev != nil {
		filter.prev.next = filter.next
	} else {
		m.httpFilters = filter.next
	}
	if filter.next != nil {
		filter.next.prev = filter.prev
	}
}

// unwrapPinnedHttpFilter unwraps the pinned http filter.
func (m *memoryManager) unwrapPinnedHttpFilter(raw uintptr) *pinedHttpFilter {
	return (*pinedHttpFilter)(unsafe.Pointer(raw))
}

// pinHttpFilterInstance pins the http filter instance to the memory manager.
func (m *memoryManager) pinHttpFilterInstance(filterInstance HttpFilterInstance) *pinedHttpFilterInstance {
	m.httpFilterInstancesMutex.Lock()
	defer m.httpFilterInstancesMutex.Unlock()
	item := &pinedHttpFilterInstance{obj: filterInstance, next: m.httpFilterInstances, prev: nil}
	if m.httpFilterInstances != nil {
		m.httpFilterInstances.prev = item
	}
	m.httpFilterInstances = item
	return item
}

// unwrapPinnedHttpFilterInstance unwraps the pinned http filter instance from the memory manager.
func (m *memoryManager) unpinHttpFilterInstance(filterInstance *pinedHttpFilterInstance) {
	m.httpFilterInstancesMutex.Lock()
	defer m.httpFilterInstancesMutex.Unlock()
	if filterInstance.prev != nil {
		filterInstance.prev.next = filterInstance.next
	} else {
		m.httpFilterInstances = filterInstance.next
	}
	if filterInstance.next != nil {
		filterInstance.next.prev = filterInstance.prev
	}
}

// unwrapRawPinHttpFilterInstance unwraps the raw pointer to the pinned http filter instance.
func unwrapRawPinHttpFilterInstance(raw uintptr) *pinedHttpFilterInstance {
	return (*pinedHttpFilterInstance)(unsafe.Pointer(raw))
}
