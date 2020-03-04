#!/bin/bash

rev=`svn info | grep Rev: | tail -c4 | head -c3`
head -c-4 revision.h > .tmpfile
echo $rev >> .tmpfile
cat .tmpfile > revision.h
rm .tmpfile

echo "current revision is $rev"