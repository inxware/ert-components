# NVIDIA NGC Authentication

Building the Docker image for this target requires pulling the NVIDIA NGC base
image, which requires a free NVIDIA developer account.

## One-time setup

### 1. Create an NVIDIA developer account

Go to https://ngc.nvidia.com and sign up for a free account if you do not
already have one.

### 2. Generate an API key

1. Log in to https://ngc.nvidia.com
2. Click your username in the top-right corner and select **Setup**
3. Under **API Key**, click **Generate API Key**
4. Copy the key — it is only shown once

### 3. Find your username

Your NGC username for `docker login` is always the literal string:

```
$oauthtoken
```

This is not your email address or account name — it is a fixed token placeholder
used by all NGC users.

### 4. Log in to the NGC registry

```bash
docker login nvcr.io --username '$oauthtoken' --password '<your-api-key>'
```

This only needs to be done once per machine. Docker stores the credentials in
`~/.docker/config.json`.

## After authentication

Build the local Docker image as normal:

```bash
echo "linux_arm64_lvgl_jetson_nano_ngc" > TARGET.cfg
make build_docker_local
```

Or build and publish to Docker Hub (CI/CD):

```bash
make publish_docker_image
```

Once the image is published to Docker Hub, other developers only need Docker Hub
access to use `make target_buildenv` — they do not need an NGC account.
