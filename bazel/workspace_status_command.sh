# The code below presents an implementation that works for git repository
git_rev=$(git rev-parse HEAD) || exit 1
echo "BUILD_SCM_REVISION ${git_rev}"
