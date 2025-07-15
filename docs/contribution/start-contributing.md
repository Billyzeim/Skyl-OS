# Start Contributing

To ensure consistency among contributors we are using a **docker-based** approach.
In this document I will hold your hand through the development environment set-up process.

## Docker Usage

### 1. Downloading Docker

Docker will make sure all developers share the same version of the same tools installed on their machines. 
Doing this is a good practice that ensures reproducability and consistency. [Here](https://docs.docker.com/get-started/get-docker/) is a downlaod link.
It is also highly recommended that you work on a Linux machine, Debian suits best.

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

## Writing Documentation

While developing your own features it is very important for other people to userstand how your code works.</br>

### 1. Documentation Files

Under [docs](docs) there should be a file stuctrue starting from more abstract structures of the project such as `kernel` or `bootloader`.</br>
Under them there should lie folders for grouped parts of the project that handle common things like `terminal-graphics` or `process-handling` for the `kernel`.</br>
If further categorization is needed to provide clarity it should be pursued. When an appropriate depth is reached there should be a `feature-name.md` file that contains the following information:</br>
1. A general description of what the feature does and why it is needed (keep this one concise).
2. A usage guide for different functions such as types and use cases (examples really make things more clear).
3. A more detailed explaination of how and why the code works (techniques and algorithms used).
4. Possible limitations and warnings about your code.</br>

>[!TIP]
>You are encouraged to link your code to help readers locate it.

### 2. Comments

Please, please, please use comments inside your code to clarify functionality.</br>
Documentation is usually not enough for developers to understand how your code will behave in certain contexts, thus leading them into taking a peek of your code.<br>
Leaving comments behind will ceratinly make their job easier since certain syntaxes might resemble to sorcery!

By now you should be ready to start contributing... maybe... I am not sure...</br>
However this is why you are also encouraged to make changes to files like this one. </br>
All the above apply!

### **Happy Coding :)**
