git filter-branch --force --index-filter \
'git rm --cached --ignore-unmatch ./rec_one/*' \
--prune-empty --tag-name-filter cat -- --all
