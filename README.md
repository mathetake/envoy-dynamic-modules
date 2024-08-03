# Envoy + dynamic modules 

This is the repository that hosts the custom C++ code that is compiled togather with the Envoy Proxy code to allow dynamically loadable modules to be loaded as HTTP filters.

We are already in the process of upstreaming the work here to the main Envoy Proxy repository. Meanwhile, this repository serves as a playground to experiment with the dynamic modules feature.

To align the development with the upstream, all source code is put under [source/extensions/dynamic_modules](source/extensions/dynamic_modules) directory.
See the [README.md](source/extensions/dynamic_modules/README.md) file in that directory for more details.

