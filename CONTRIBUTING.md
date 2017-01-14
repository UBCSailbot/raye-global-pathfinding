# How to contribute

If your changes are significant then make a new branch that is named after the JIRA issue, in the format SOFT-XXX-brief-description

e.g. SOFT-753-boxes2headings

## Making Changes

* Create a feature branch from where you want to base your work.
  * This is usually the master branch.
  * To quickly create a feature branch based on master; `git checkout -b
    feature/my_contribution master`.
  * **Do not work directly on the `master` branch.**
* Make commits of logical units.
* Check for unnecessary whitespace with `git diff --check` before committing.
* Make sure your commit messages are accurate and coherent.
