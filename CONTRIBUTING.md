
| Table of Contents |
|-------------------|
| [Basics](#basics) |
| [Setting up your Environment](#setting-up-your-environment) |
| [Using Containerization](#using-containerization) |
| [MacOS](#macos) |
| [Commenting](#commenting) |
| [Optimization](#optimization) |
| [Code Style](#code-style) |



## Basics
When you work on anything please create an issue for it or comment on the appropriate issue to avoid duplication of work. 
Please be descriptive in your issues, posts, and PRs and describe what you are trying to achieve.

## Setting up your Environment

1. Clone this repo to a Linux Machine or a WSL Distro on a Windows Machine
	- More information about WSL can be found [here](https://code.visualstudio.com/docs/remote/wsl)
2. Install the Dev Dependencies by running `dev-install.sh` in the root directory
3. Run `cmake .` from the root directory

### Using Containerization

You can also use Docker or Podman to create a containerized development environment. This is the recommended way to work on the project as it ensures a consistent environment for all developers. We don't require any specific method, but if you choose docker instead of podman be sure to adjust the permissions so that it does not run as root. The project has `.devcontainer` already set up and configured to suite most environments (except MacOS - check the [MacOS section](#macos) for more info). To use the containerized environment, follow these steps:
1. Install Docker or Podman on your machine.
2. In vscode, install the "Remote - Containers" extension.
3. Install the "Dev Containers" extension.
4. If you choose to use Podman, you will need to configure the vscode `settings.json` file to use Podman instead of Docker. Add the following line to your `settings.json` file:
	```json
	"dev.containers.dockerPath": "podman",
	"dev.containers.mountWaylandSocket": false
	```
5. Open the project in vscode.
6. Press `F1` and select `Remote-Containers: Reopen in Container`.

### MacOS

f you're using MacOS, podman runs a full VM and runs the containers in that. Which means you need to alter the Dockerfile to alter the `gid` and `uid` of the user your environment is running with (in our case, `vscode`). Otherwise, you won't be able to write to the folder when it's mounted transitively through the VM, because the VM hands it the `uid` and `gid` of the local VM account instead of the host.

To do this, you need to find out your `uid` and `gid` by running `id -u` and `id -g` in your terminal. Then, you need to alter the `Dockerfile` in the `.devcontainer` folder to add the following lines just before the first `RUN` line. Assuming your `uid` and `gid` are both `501`, you would add the following lines:

```Dockerfile
RUN groupmod --gid 501 vscode && usermod --uid 501 --gid 501 vscode && chown -R 501:501 /home/vscode
```

## Commenting
We use the Doxygen format of commenting and documentation generation. For styling and use, please see: [Doxygen Docs](https://www.doxygen.nl/manual/docblocks.html). Not everything needs to be commented, but complex topics should have comments added to ease reading them. Try to be "self documenting" by using descriptive variable names, short function bodies, and adding spacer lines between ideas where appropriate. If you think someone might wonder 'why' something was done a certain way and you have a good reason, that's a good spot for  a friendly comment. Provide supporting information for functions (and their parameters) and classes so community members can easily understand how and why to use the tools available.

## Optimization
Though the goal of modernizing will include optimization, avoid premature optimization without reason. The program is quite large and old, and as such some things may react poorly to optimizing in ways that are not easily noticed. If you optimize code, be certain the function and make sure to spin up a server instance and validate your changes.

## Code Style
Generally this isn't much of an issue, but try to be consistent and avoid sloppy work. One of the goals is to achieve reasonably 
easy to read code to extend the life of the code base. As such:
* Indent with tabs, not spaces. Many Bothans died to bring us this rule.
* If you work on the website/PHP section, please follow PSR-12 but do note the tabs instead of spaces https://www.php-fig.org/psr/psr-12/
* For definitions, use `#ifdef ...` when one condition is required.
* Follow modern C++ convention, properly utilizing header files.
* Try to stay SOLID and DRY (which can be difficult in a 20yr old code base).
* Keep lines a reasonable length, typically under 120 characters unless absolutely needed. 
* Name things reasonably. If you need a guide: https://docs.microsoft.com/en-us/dotnet/standard/design-guidelines/general-naming-conventions
* Put braces on their own lines:
    ~~~~cpp
    public void Something()
    {
        // code
    }
    ~~~~
