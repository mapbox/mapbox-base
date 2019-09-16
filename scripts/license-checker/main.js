// @flow

import crypto from 'crypto';
import fs from 'fs';
import path from 'path';

const Git = require('nodegit');
const yargs = require('yargs');

type Action =
    | 'file'
    | 'text'
    | 'ignore';

type Entry = {|
    name: ?string,
    path: string,
    hash: string,
    licenseFile: string,
    licenseText: ?string,
    action: Action
|};

type LockFile = Array<Entry>;

function hashFile(path): ?string {
    let hash = crypto.createHash('sha256');

    try {
        const data = fs.readFileSync(path);
        hash.update(data);
        return hash.digest('hex');
    } catch (err) {
        return undefined;
    }
}

function loadLockFile(repoPath): Promise<LockFile> {
    try {
        const file = fs.readFileSync(path.join(repoPath, "license-lock"));
        return Promise.resolve(JSON.parse(file.toString()));
    } catch (err) {
        if (err.code === 'ENOENT') {
            return Promise.resolve([]);
        }
        return Promise.reject(err);
    }
}

function saveLockFile(lockFile, repoPath) {
    fs.writeFileSync(path.join(repoPath, "license-lock"), JSON.stringify(lockFile, null, 2));
}

function generateLockFileForRepo(repoPath): Promise<LockFile> {
    function findLicenseFile(repoPath) {
        const fileNames = [
            'LICENSE.md',
            'LICENSE.txt',
            'LICENSE',
            'license.txt',
            'LICENSE_1_0.txt',
            'UNLICENSE'
        ];

        for (let i = 0; i < fileNames.length; i++) {
            if (fs.existsSync(path.join(repoPath, fileNames[i]))) {
                return fileNames[i];
            }
        }
        return undefined;
    }

    return Git.Repository.open(repoPath)
        .then((repo) => {
            var paths = []
            return Git.Submodule.foreach(repo, (sub) => {
                paths.push(sub.path());
            }).then((_) => {
                return paths;
            })
        }).then((paths) => {
            const lockFile = paths.map((p) => {
                const submodulePath = path.join(repoPath, p);
                const licenseFile = findLicenseFile(submodulePath);
                return {
                    path: p,
                    licenseFile: licenseFile,
                    hash: licenseFile ? hashFile(path.join(submodulePath, licenseFile)) : undefined,
                    action: 'file'
                };
            })
            return Promise.resolve(lockFile);
        });
}

function lockFileToString(lockFile) {
    return JSON.stringify(lockFile, null, 2);
}


function checkLicenses(repoPath) {
    Promise.all([loadLockFile(repoPath), generateLockFileForRepo(repoPath)])
        .then((lockFiles) => {
            const lockFile = lockFiles[0];
            const newLockFile = lockFiles[1];

            lockFile.forEach((entry) => {
                if (entry.hash === undefined || entry.licenseFile === undefined) {
                    console.error(`License information missing for submodule ${entry.path}`);
                    process.exit(1);
                }
                const hash = hashFile(path.join(repoPath, entry.path, entry.licenseFile)) || "";
                if (hash !== entry.hash) {
                    console.error(`License for submodule ${entry.path} changed!`);
                    console.error(`  old hash: ${entry.hash}`);
                    console.error(`  new hash: ${hash}`);
                    process.exit(1);
                }
                if (entry.action === 'text' && !entry.licenseText) {
                    console.error(`Submodule ${entry.path} has action \'text\' but licenseText is not defined`);
                    process.exit(1);
                }
            });

            newLockFile.forEach((newEntry) => {
                const oldEntry = lockFile.find((e) => e.path === newEntry.path);
                if (oldEntry === undefined) {
                    console.error(`No license entry for submodule ${newEntry.path}`);
                    process.exit(1);
                }
            });
        })
        .catch((err) => {
            console.error(err);
            process.exit(1);
        });
}

function licenseText(projName, repoPath) {
    loadLockFile(repoPath)
        .then((lf) => {
            const license = lf.filter((e) => e.action !== 'ignore').map((entry) => {
                const text = entry.action === 'text'
                        ? entry.licenseText
                        : fs.readFileSync(path.join(repoPath, entry.path, entry.licenseFile));

                const name = entry.name || path.basename(entry.path);

                return [
                    `${projName} uses portions of ${name}`,
                    '',
                    text
                ].join('\n');
            }).join('\n===========================================================================\n\n');

            console.log(license);
        })
        .catch((err) => {
            console.log(err);
            process.exit(1);
        });
}


yargs
    .command('generate [path]',
             'automatically generate a new lock file for repository',
             (yargs) => yargs.positional('path', {describe: 'path to git repository', default: './'}),
             (argv) => generateLockFileForRepo(argv.path).then((lf) => console.log(lockFileToString(lf))))
    .command('check [path]',
             'check licenses against repository\'s lock file',
             (yargs) => yargs.positional('path', {describe: 'path to git repository', default: './'}),
             (argv) => checkLicenses(argv.path))
    .command('license <name> [path]',
             'write a license file to standard output',
             (yargs) => yargs
                 .positional('name', {describe: 'project name used in the license file'})
                 .positional('path', {describe: 'path to git repository', default: './'}),
             (argv) => licenseText(argv.name , argv.path))
    .argv;
