#script for build on different platforms with different compilers maf.
#Using this prevent from using cmake. (support compilers are gcc on linux and mac, mingw and visual studio 2008 in windows)

import os, sys
import getopt
import shutil
import httplib, urlparse, string
import platform

import urllib2

from base64 import encodestring, decodestring

currentPathScript = os.path.abspath(os.path.split(os.path.realpath(__file__))[0])
param = {}
paramError = {}
bt_home = "http://www.biomedtown.org"
    
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

def download64BitLinux():
    library = '/biomed_town/MAF/MAF3%20Floor/download/foundation_libraries/Linux_i386/foundation_linux_64'
    url = bt_home + library
    localFileName = 'MAF_Foundation_Libs.tar.gz'
    downloadFromLink(url, localFileName)
    pass
    
def download32BitLinux():
    library = '/biomed_town/MAF/MAF3%20Floor/download/foundation_libraries/Linux_i386/foundation_libs'
    url = bt_home + library
    localFileName = 'MAF_Foundation_Libs.tar.gz'
    downloadFromLink(url, localFileName)
    pass
    
def download64BitMacOSX():
    library = '/biomed_town/MAF/MAF3%20Floor/download/foundation_libraries/macosx/foundation_libs_snowleopard'
    url = bt_home + library
    localFileName = 'MAF_Foundation_Libs.tar.gz'
    downloadFromLink(url, localFileName)
    pass
    
def download32BitMacOSX():
    library = '/biomed_town/MAF/MAF3%20Floor/download/foundation_libraries/macosx/foundation_libs'
    url = bt_home + library
    localFileName = 'MAF_Foundation_Libs.tar.gz'
    downloadFromLink(url, localFileName)
    pass

def download32BitWin32VS2008():
    library = '/biomed_town/MAF/MAF3%20Floor/download/foundation_libraries/Win32/MAF3_Foundation_Libs.zip'
    url = bt_home + library
    localFileName = 'MAF_Foundation_Libs.zip'
    downloadFromLink(url, localFileName)
    pass

def download32BitWin32Mingw():
    library = '/biomed_town/MAF/MAF3%20Floor/download/foundation_libraries/Win32/foundation_libs'
    url = bt_home + library
    localFileName = 'MAF_Foundation_Libs.zip'
    downloadFromLink(url, localFileName)
    pass

def chunk_report(bytes_so_far, chunk_size, total_size):
   percent = float(bytes_so_far) / total_size
   percent = round(percent*100, 2)
   sys.stdout.write("Downloaded %d of %d bytes (%0.2f%%)\r" % 
       (bytes_so_far, total_size, percent))

   if bytes_so_far >= total_size:
      sys.stdout.write('\n')

def chunk_read(response, chunk_size=8192, report_hook=None):
   total_size = response.info().getheader('Content-Length').strip()
   total_size = int(total_size)
   bytes_so_far = 0

   total = ""
   while 1:
      chunk = response.read(chunk_size)
      total += chunk
      bytes_so_far += len(chunk)

      if not chunk:
         break

      if report_hook:
         report_hook(bytes_so_far, chunk_size, total_size)

   return total
   
def downloadFromLink(url, localFileName):
   response = urllib2.urlopen(url);
   data = chunk_read(response, report_hook=chunk_report)
   f = open(localFileName, 'w')
   f.write(data)
   f.close()

def errhandler():
   print "Unrecognized system."

def downloadFromBiomedtown():    
    platformOS = {
    "64bit-linux2": download64BitLinux,
    "32bit-linux2": download32BitLinux,
    "64bit-darwin": download64BitMacOSX,
    "32bit-darwin": download32BitMacOSX,
    "64bit-linux2-gcc4": download64BitLinux,
    "32bit-linux2-gcc4": download32BitLinux,
    "64bit-darwin-gcc4": download64BitMacOSX,
    "32bit-darwin-gcc4": download32BitMacOSX,
    #"64bit-win32" : download64BitWin32,
    "32bit-win32" : download32BitWin32VS2008,
    "32bit-win32-vs2008" : download32BitWin32VS2008,
    "32bit-win32-mingw" : download32BitWin32Mingw,
    }
    
    #construct variable
    value = platform.architecture()[0] + "-" + str(os.sys.platform).lower()
    key = 'compiler'
    if(key in param):
        value = value + "-" + param[key]
    print "Detected System..." + value
    try:
        print "Download in Progress..."
        platformOS.get(value,errhandler)()
        print "Download Completed"
    except Exception, e:
        print "Error trying to launch the download from biomedtown..." , e

def createEnvironmentVariables():
    print "Create Environment variables..."
    pass

def gitDownloadFromCTKRepository():
    print "Download from CTK repository..."
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
    print "Extract module from CTK..."
    mafRootDir = currentPathScript
    mafEventBusDir = os.path.join(mafRootDir, "ctkEventBus")
    ctkLocalDir = os.path.join(param['foundation-output-dir'], "CTK")
    ctkEventBus = os.path.join(ctkLocalDir, "Plugins", "org.commontk.eventbus", "mafEventBus")
    if os.path.exists(mafEventBusDir):
        shutil.rmtree(mafEventBusDir)
    
    shutil.copytree(ctkEventBus, mafEventBusDir)

    
def install():
    createFoundationDirectories()
    downloadFromBiomedtown()
    createEnvironmentVariables()
    gitDownloadFromCTKRepository()
    extractFromLocalCTK()
          
def usage():
    print "python mafBuild.py [-h] [-f foundation-output-dir] [-g git-path]"
    print "-h, --help                            show help (this)"
    print "-f, --foundation-output-dir           select where set foundation library"
    print "-c, --compiler=                       select the compiler (used only if process is code)"
    print "-g, --git-path                        set directory of git"
    print 
    
def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hf:g:", ["help", "foundation-output-dir=", "compiler=", "git-path="])
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)

    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-c", "--compiler"):
            acceptance = ["vs2008","mingw","gcc4"]
            if(a in acceptance):
                param['compiler'] = a
            else:
                paramError['compiler'] = a
        elif o in ("-f", "--foundation-output-dir"):
            param['foundation-output-dir'] = os.path.abspath(os.path.normpath(a))
        elif o in ("-g", "--git-path"):
            param['git-path'] = os.path.abspath(os.path.normpath(a))
        else:
            assert False, "unhandled option"
    
    if(len(paramError) != 0 ):
        for k, v in paramError.iteritems():
            print v, " is not a valid " , k
        usage()
        exit()
    
    install()
    
if __name__ == "__main__":
  main()