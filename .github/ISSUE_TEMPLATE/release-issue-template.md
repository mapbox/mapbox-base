---
name: Make a release
about: Checklist for a new release of mapbox-base
title: "[release] Release <version>"
labels: release
assignees: ''

---

# Release <version>

- Releaser: `@<releaser-github-handle>`

## ☑ Release checklist

### QA
- [ ] Discuss with the team and stakeholders to make sure the release is good to go
- [ ] Perform final testing to assure QA; make sure all bots are green

### Changelog
- [ ] Update the changelog and merge it to master
- Until the process is automated, do this by comparing the difference between the previous `<release-name>` branch and what is on `master`, using this URL as a reference: `https://github.com/mapbox/mapbox-base/compare/<previous-release>...master`

### Create release branch
- [ ] Create a branch off mapbox-base `master` with the same [branch protections](https://github.com/mapbox/mapbox-base/settings/branches) as `master` for this release. Name the branch: `release-v<version>`)
- The easiest way to cut the branch and apply branch protection is by using [`github-release-tools`](https://github.com/mapbox/github-release-tools/). Once you have installed this, you can cut the branch and apply the branch protections like so:

```
$ git checkout master && git pull origin master
$ git checkout -b release-v<version> && git push origin release-v<version>
$ branch-permissions release-v<version> master
```

### Create GitHub release
- [ ] Create a GitHub release on https://github.com/mapbox/mapbox-base/releases
  - [ ] Create a git tag by giving the release a [version](https://semver.org/). Tag the release: `v<version>`).
  - [ ] Target should point at the release-branch
  - [ ] Add the changelog entries for the release to the `Describe this release` section

### Patching the release
Any changes made to the release branch **after** the release branch has been created *need to be cherry picked* into the release branch. This is the responsibility of the PR author.

/cc @mapbox/cpp @mapbox/core-sdk @mapbox/gl-native @mapbox/navigation @mapbox/search-sdk @mapbox/vision-core
