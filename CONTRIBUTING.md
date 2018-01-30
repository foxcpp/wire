# Contributing

First off, thank you for considering contributing to libwire. It's people
like you that make libwire such a great thing.

Following these guidelines helps to communicate that you respect the time of
the developers managing and developing this open source project. In return,
they should reciprocate that respect in addressing your issue, assessing
changes, and helping you finalize your pull requests.

# What to do?

* Ask a question
* Review a pull request
* Fix an issue
* Update the documentation

# What **not** to do?

See [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

# First Contribution

Unsure where to begin contributing? You can start by looking through
these [easy](https://github.com/foxcpp/wire/labels/easy) and
[help-wanted](https://github.com/foxcpp/wire/labels/help%20wanted) issues:
Easy issues - issues which should
only require a few lines of code, and a test or two. Help wanted issues -
issues which should be a bit more involved than beginner issues. Both issue
lists are sorted by total number of comments. While not perfect, number of
comments is a reasonable proxy for impact a given change will have.

Also you may want to look here: http://makeapullrequest.com/.

# Pull requests

1. Fork & create a branch

If this is something you think you can fix, then fork this repository and
create a branch with a descriptive name.

2. Did you find a bug?

* Ensure the bug was not already reported by searching all issues.

* If you're unable to find an open issue addressing the problem, open a new
  one. Be sure to include a title and clear description, as much relevant
  information as possible.

3. Implement your fix or feature

At this point, you're ready to make your changes!

4. Get the style right

Your patch should follow the same conventions & pass the same code quality
checks as the rest of the project.

* Run `make check`.
* Run `make format`.

5. Make a Pull Request

At this point, you should switch back to your master branch and make sure it's up to date with libwire's master branch:

```
git remote add upstream git@github.com:foxcpp/wire.git
git checkout master
git pull upstream master
```

Then update your feature branch from your local copy of master, and push it!

```
git checkout your-branch-name-here
git rebase master
git push --set-upstream origin your-branch-name-here
```

6. Keeping your Pull Request updated

If a maintainer asks you to "rebase" your PR, they're saying that a lot of code
has changed, and that you need to update your branch so it's easier to merge.

To learn more about rebasing in Git, there are a lot of good resources but
here's the suggested workflow:

```
git checkout your-branch-name-here
git pull --rebase upstream master
git push --force-with-lease your-branch-name-here
```

# Bug Reports

1. Determine if your bug is really a bug

Check the documentation, make sure you are doing everything right.

2. Make sure your bug hasn't already been reported

Search through the appropriate Issue tracker. If a bug like yours was found,
check if you have new information that could be reported to help
the developers fix the bug.

3. Check if you're using the latest version

A bug could be fixed by some other improvements and fixes - it might not have an
existing report in the bug tracker. Make sure you are using the latest release.

5. Collect information about the bug

To have the best chance of having a bug fixed, we need to be able to easily
reproduce the conditions that caused it.

* Include any relevant output.
* We also need to know what platform you're running.
* Compile with debug information, run `gdb`, stop on error, run `bt`.
* If you're reporting a race condition or a deadlock, backtrace can be hard to
  get or might not be that useful. Try to inspect the process to get more
  diagnostic data. Some ideas:

  - Run [`strace`](https://en.wikipedia.org/wiki/Strace), [`ltrace`](https://en.wikipedia.org/wiki/Ltrace), [`lsof`](https://en.wikipedia.org/wiki/Lsof).
  - Play around with `gdb` if you know what to do.

