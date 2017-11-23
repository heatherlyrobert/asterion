#!/bin/bash

grep "^[a-z][A-Za-z]  [A-Za-z][A-Za-z]" HTAG.tags    > call_graph
printf "\n"                                         >> call_graph
cat  HTAG.flow                                      >> call_graph

