#!/bin/bash

# Need to give permission to execute this script, by running the following command
# chmod +x sync-fork.sh

# Variables
UPSTREAM_URL="https://github.com/uwe-scct/ufcfvk-15-2-internet-of-things-2425-iot2425-graded-ws2-iot-graded-ws-2"
BRANCH="main"  

# Add upstream remote if it doesn't exist
if ! git remote | grep -q upstream; then
  git remote add upstream $UPSTREAM_URL
fi

# Fetch upstream changes
git fetch upstream

# Checkout to the branch you want to update
git checkout $BRANCH

# Merge upstream changes into your local branch
git merge upstream/$BRANCH -m "Sync with upstream"

# Push changes to your fork
git push origin $BRANCH

echo "Fork synced with upstream successfully!"
