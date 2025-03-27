# Start Contributing

To ensure consistency among contributors we are using a **docker-based** approach.
In this document I will hold your hand through the development environment set-up process.

## 1. Downloading Docker

Docker will make sure all developers share the same version of the same tools installed on their machines. 
Doing this is a good practice that ensures reproducability and consistency. [Here](https://docs.docker.com/get-started/get-docker/) is a downlaod link.
It is also highly recommended that you work on a Linux machine, Debian sutis best.

We will not conduct a thorough docker tutorial so you can feel free to mess arround with the docker [documentation](https://docs.docker.com/).

## 2. Building the Docker Container

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

> [!IMPORTANT]
> Make sure the docker use is evelated or that you use `sudo` for the above commands.