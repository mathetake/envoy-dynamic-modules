git_rev=$(git rev-parse HEAD) || exit 1
echo "BUILD_SCM_REVISION ${git_rev}"
