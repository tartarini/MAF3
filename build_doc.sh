if [ -d /scssvn/doc.openmaf.org/Doc ]; then
  rm -rf /scssvn/doc.openmaf.org/Doc;
fi

#if [ -d ~/QAResults ]; then
#  rm -rf ~/QAResults;
#fi

~/doxygen-1.5.9/bin/doxygen MAF3Doxyfile;
~/doxygen-1.5.9/bin/doxygen MAF3DoxyfileWithTests;
#mv Doc ~/Doc;

echo "DOXY SUCCESSFUL"
