#!/bin/bash
function pyexec() {
echo "$(/usr/bin/python -c 'import sys, urllib as ul; print ul.unquote_plus(sys.stdin.read())')"    
#echo "$(/usr/bin/python -c 'import sys; exec sys.stdin.read()')"
}
echo $(echo "je+suis+ici" | pyexec);

