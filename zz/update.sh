#!/bin/sh

   #******************************************
   # echo "# stm32_vga" >> README.md
   git init
   git status
   git add .
   git commit -m "$(date "+%Y-%m-%d")"
   git branch -M master
   git remote add origin git@github.com:Alex2269/stm32_vga.git
   git push -u origin master
   git pull
   #******************************************

   #******************************************
   git status
   git add .
   git commit -m "$(date "+%Y-%m-%d")"
   git push -u origin master
   git pull
   #******************************************

   #******************************************
   #  git log
   #  git reset --hard  <commit-hash-xxx>
   #  git reset --hard  <commit-hash-xxy>
   #  git reset --hard  <commit-hash-xxz>
   #  git push origin HEAD --force
   #******************************************
