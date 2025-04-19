#!/bin/bash

# Variables
GITHUB_USERNAME="rolln7drktayau"
REPO_NAME="HSI_Loiciel_BCGV"
GITHUB_TOKEN="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"
GITIGNORE_CONTENT="*.log
*.pyc
__pycache__/
.env
.DS_Store
push_to_github.sh"

# Create remote repo on GitHub
curl -u "$GITHUB_USERNAME:$GITHUB_TOKEN" https://api.github.com/user/repos -d "{\"name\":\"$REPO_NAME\",\"private\":true}"

# Initialize git repo if not already
git init

# Create .gitignore
# echo -e "$GITIGNORE_CONTENT" > .gitignore

# Add files
git add .
git commit -m "Initial commit"

# Add remote and 
git branch -M main
git remote add origin https://$GITHUB_USERNAME:$GITHUB_TOKEN@github.com/$GITHUB_USERNAME/$REPO_NAME.git
git push -u origin main

# git remote add origin https://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@github.com/rolln7drktayau/SecIOT.git
# git push -u origin main