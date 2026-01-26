# Activity 1: CSCT Cloud Server Configuration

[<-- back](prep-worksheet0.md#activities)

In this activity you will set up the CSCT Cloud Server. 

**REMEMBER: If working in UWE Machines**

If you are working on UWE machines, make sure you store your files, including any configuration file, repositories or VS Code extension, in the **H://** drive. This way it will be stored in your Windows account and not in the specific UWE machine you are working on. This will enable you can access your files from any other UWE machine.


## 1. Create your SSH keys

Follow [this guide](https://xerte.uwe.ac.uk/USER-FILES/8660-me-perezhernandez-site/media/csct-cloud-ssh-keys-config.pdf) to configure your SSH keys that will enable authentication with the CSCT Cloud service. 

## 2. VS Code Setting Sync

To be able to keep your VS Code configuration across multiple machines, you will need to enable setting sync as indicated in the following link:

[VS Code Setting Sync](https://code.visualstudio.com/docs/editor/settings-sync#_turning-on-settings-sync)


You can use either Github or Microsoft accounts but remember every time you change to a different machine (UWE or personal), sign in with the same account and your VS Code configuration will be synced. 

## 3. Install VS Code Remote Development Pack

As we are going to use the CSCT Cloud to run our worksheets, we need to first set up the tools we will need to access the cloud and work remotely from within VS Code. 

Let's install the [VS Code Remote Development Pack](https://code.visualstudio.com/docs/remote/remote-overview#_remote-development-extension-pack).


Once installed, go to the [Command Palette](https://code.visualstudio.com/docs/getstarted/userinterface#_command-palette) and type *Remote SSH: Open SSH Configuration File*. Click on this option and then select *Settings*. It will take you to a field to type the path to your .ssh configuration file:

```shell
Remote.SSH: Config File
The absolute file path to a custom SSH config file.
```

**If you are using a UWE machine, the path you need to set should be something like: ``H:\.ssh\config``.**

## 4. Connect to the CSCT Cloud Server

With the ssh keys set and remote development pack, you can [connect to the host using Remote.SSH](https://code.visualstudio.com/docs/remote/ssh#_connect-to-a-remote-host). Your connection string should look as follows, just replace ``first-name.last-name@live.uwe.ac.uk`` with your UWE email.

```shell
ssh first-name.last-name@live.uwe.ac.uk@csctcloud.uwe.ac.uk
```

The host platform is ``Linux``.

As you have configured your ``.ssh/config`` file in the previous step, VS Code should take the ssh keys configuration from the folder set for authentication.

The new host (server) will open in a new VS Code window.

Once connected to the server, [open a new terminal](https://code.visualstudio.com/docs/terminal/getting-started#_run-your-first-command-in-the-terminal), **create a new directory** for the IoT worksheets, use the commands as specified in the [Linux Commands Cheat Sheet](https://assets.ubuntu.com/v1/3bd0daaf-Ubuntu%20Server%20CLI%20cheat%20sheet%202024%20v6.pdf) under 'Directory navigation' section.

## 5. Configure your GitHub user in the CSCT Cloud Server

At the beginning of this worksheet you started working from within your client machine, either your personal computer or a UWE machine. Since you will write code on the CSCT Cloud Server and may want to commit and push it to your repositories, you need to set up your Github username and email in the CSCT Cloud Server, just run the following commands from a terminal:

```shell
  git config --global user.email "you@example.com"
  git config --global user.name "Your Name"
```


If you face any problem try [this other guide](https://xerte.uwe.ac.uk/USER-FILES/8660-me-perezhernandez-site/media/legacy-vscode-remote.pdf).

[<-- back](prep-worksheet0.md#activities)