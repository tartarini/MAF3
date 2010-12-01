#script for build on different platforms with different compilers maf.
#Using this prevent from using cmake. (support compilers are gcc on linux and mac, mingw and visual studio 2008 in windows)

import os, sys
import getopt
import shutil

currentPathScript = os.path.abspath(os.path.split(os.path.realpath(__file__))[0])
param = {}
    
def errhandler():
   print "Unrecognized compiler"

def createFoundationDirectories():
    try:
        foundationDir = param['foundation-output-dir']
    except:
        param['foundation-output-dir'] = os.path.join(currentPathScript, ".." , "MAF3_Foundation_Libs")
        foundationDir = param['foundation-output-dir']
    
    if not os.path.exists(foundationDir):
        os.makedirs(foundationDir)
    pass

def gitDownloadFromCTKRepository():
    mafRootDir = currentPathScript
    eventBusDir = os.path.join(mafRootDir, "org.commontk.eventbus")
    ctkRepository = "git://github.com/dgiunchi/CTK.git"
    ctkLocalDir = os.path.join(param['foundation-output-dir'], "CTK")
    gitExecutable = os.path.join(param['git-path'], "git")
    
    command = ""
    if not os.path.exists(ctkLocalDir):
        #clone
        command = gitExecutable + " clone " + ctkRepository + " " + ctkLocalDir
    else:
        #pull
        os.chdir(ctkLocalDir)
        command = gitExecutable + " pull"

    os.system(command)
    os.chdir(currentPathScript)
    
        
def extractFromLocalCTK():
    mafRootDir = currentPathScript
    mafEventBusDir = os.path.join(mafRootDir, "ctkEventBus")
    ctkLocalDir = os.path.join(param['foundation-output-dir'], "CTK")
    ctkEventBus = os.path.join(ctkLocalDir, "Plugins", "org.commontk.eventbus")
    if os.path.exists(mafEventBusDir):
        shutil.rmtree(mafEventBusDir)
    
    shutil.copytree(ctkEventBus, mafEventBusDir)
    
    
def install():
    createFoundationDirectories()
    gitDownloadFromCTKRepository()
    extractFromLocalCTK()
          
def usage():
    print "python mafBuild.py [-h] [-f foundation-output-dir] [-g git-path]"
    print "-h, --help							show help (this)"
    print "-f, --foundation-output-dir			select where set foundation library"
    print "-g, --git-path						set directory of git"
    print 
    
def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hf:g:", ["help", "foundation-output-dir=", "git-path="])
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)

    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-f", "--foundation-output-dir"):
            param['foundation-output-dir'] = os.path.abspath(os.path.normpath(a))
        elif o in ("-g", "--git-path"):
            param['git-path'] = os.path.abspath(os.path.normpath(a))
        else:
            assert False, "unhandled option"
    
    #complete check of parameters needed
    
    install()
    
if __name__ == "__main__":
  main()