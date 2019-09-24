set(GIT_BUILD "false")
set(GIT_IS_DIRTY "false")
set(GIT_SHA1 "")
set(GIT_BRANCH "")

if(GIT_FOUND)
  # Get current git branch, result is != 0 if this is not a git repository
  execute_process(
    COMMAND /usr/bin/git -C /Users/Ying/Study/Stanford/ClarkLab/CVC4 rev-parse --abbrev-ref HEAD
    RESULT_VARIABLE GIT_RESULT
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  if("${GIT_RESULT}" STREQUAL "0")
    set(GIT_BUILD "true")
    # Extract sha1 of HEAD
    execute_process(
      COMMAND /usr/bin/git -C /Users/Ying/Study/Stanford/ClarkLab/CVC4 rev-parse HEAD
      OUTPUT_VARIABLE GIT_SHA1
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # Result is != 0 if worktree is dirty
    # Note: git diff HEAD shows both staged and unstaged changes.
    execute_process(
      COMMAND /usr/bin/git -C /Users/Ying/Study/Stanford/ClarkLab/CVC4 diff HEAD --quiet
      RESULT_VARIABLE GIT_RESULT
    )
    if(NOT "${GIT_RESULT}" STREQUAL "0")
      set(GIT_IS_DIRTY "true")
    endif()
  endif()
endif()

configure_file(
  /Users/Ying/Study/Stanford/ClarkLab/CVC4/src/base/git_versioninfo.cpp.in git_versioninfo.cpp)
