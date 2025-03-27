# Start Contributing

To ensure consistency among contributors we are using a **docker-based** approach.
In this document I will hold your hand through the development environment set-up process.

## Docker Usage

### 1. Downloading Docker

Docker will make sure all developers share the same version of the same tools installed on their machines. 
Doing this is a good practice that ensures reproducability and consistency. [Here](https://docs.docker.com/get-started/get-docker/) is a downlaod link.
It is also highly recommended that you work on a Linux machine, Debian sutis best.

We will not conduct a thorough docker tutorial so you can feel free to mess arround with the docker [documentation](https://docs.docker.com/).

### 2. Building the Docker Container

Before writing any code we need to make sure we have built our docker container. 
You can use the following command to do so:
```
docker build -t skylos .
```
You will probably only need to use the above command again if the Dockerfile is subject to any changes.

Now you can use the following command when you need to run your container image:
```
docker run --rm -it -v $(pwd):/os skylos
```

> [!T]
> Make sure the docker use is evelated or that you use `sudo` for the above commands.

## Branching Out

### 1. Branching Rules

Every new features should be developed indepentently on its own branch. This will ensure different features have no interference with eachother during development.
When a feature is ready and very importantly functional, the developer should create a merge request with another branch (the main branch if the feature is ready for deployment).
Whenever a merge is attempted there will almost certainly be some conflicts between the two versions of the code base. 
It is the responsibility of the developer who initiated the merge attempt to resolve these conflict before submitting the request, however feel free to ask for help; this is a learning project!

> [!TIP]
> I strongly recommend that you look up about git and its good practices!</br>
> Feel free to use [this](https://education.github.com/git-cheat-sheet-education.pdf) cheat-sheet.

### 2. Managing Branches

You can use the following command to create a new branch on your ***local repository***:
```
git branch <branch-name>
```
Please use names discriptive of the feature you are developing on your branch.

To switch between branched you can use the ```git checkout <branch-name>``` command.

>[!WARNING]
>Changes will only apply on your machine before you stage, commit and push them to the origin.</br>
>Refer to git's documentation to learn how this works.

Make sure commits have appropriately discriptive titles and discriptions.
