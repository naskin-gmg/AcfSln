#!/bin/bash
if [ ! -d "../../.git/hooks" ]; then
    echo "Error: .git/hooks directory not found"
    exit 1
fi

cp ./post-merge ../../.git/hooks/post-merge || {
    echo "Error: Failed to copy post-merge hook"
    exit 1
}

chmod +x ../../.git/hooks/post-merge || {
    echo "Error: Failed to make post-merge hook executable"
    exit 1
}

echo "Hook installed!"
