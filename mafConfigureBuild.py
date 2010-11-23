#script for build on different platforms with different compilers maf.
#Using this prevent from using cmake. (support compilers are gcc on linux and mac, mingw and visual studio 2008 in windows)

import os, sys
import getopt

currentPathScript = os.path.split(os.path.realpath(__file__))[0]
param = {}

def buildVS2008():
    print "Starting to Build MAF with VS2008"
    module = param['module']
    suffix = 'Test'
    
    flagTest = param['enable-test']

    if(flagTest):
        dirList = os.listdir(module)
        for item in dirList:
            val =  'maf' in item
            isTest = 'Test' in item
            isADir = os.path.isdir(os.path.join(module,item))
            exists = os.path.exists(os.path.join(module,item))
            if (isTest == True and val and isADir):
                os.chdir(os.path.join(currentPathScript, module, item))
                fileBuild = "buildVS2008_" + item + ".bat"
                s = open(fileBuild, 'w')        
                s.write("IF EXIST " + item +"_d.vcproj devenv " + item +"_d.vcproj /clean\n")
                s.write("REM IF EXIST " + item +".vcproj devenv "+ item +".vcproj /clean\n")
                s.write("\n")
        
                s.write("qmake.exe -r -tp vc -spec win32-msvc2008\n")
        
                s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
                s.write("devenv " + item +"_d.vcproj /build debug /out build_log_" + item +"_d.txt\n")
                #s.write("type build_log_" + item + "_d.txt\n")
                s.write("REM devenv "+ item + "_d.vcproj /build debug /out build_log_" + item +".txt\n")
                s.write("REM type build_log_" + item + ".txt\n")
                s.write("\n")
                s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
                s.write("\n")
                s.write("echo \"BUILD SUCCESSFUL\" %1\n")
                s.write("\n")
                s.write(":END\n")
                s.close()
    
                #script execution
                os.system(fileBuild)
                #script removing
                os.remove(fileBuild)
                
                os.chdir(currentPathScript)
    else:
        #script generation
        os.chdir(os.path.join(currentPathScript, module))
        fileBuild = "buildVS2008_" + module + ".bat"
        s = open(fileBuild, 'w')  
        
        s.write("IF EXIST " + module +"_d.vcproj devenv " + module +"_d.vcproj /clean\n")
        s.write("REM IF EXIST " + module +".vcproj devenv "+ module +".vcproj /clean\n")
        s.write("\n")
        s.write("qmake.exe -r -tp vc -spec win32-msvc2008\n")  
    
        s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
        s.write("devenv " + module +"_d.vcproj /build debug /out build_log_" + module +"_d.txt\n")
        #s.write("type build_log_" + module + "_d.txt\n")
        s.write("REM devenv "+ module + "_d.vcproj /build debug /out build_log_" + module +".txt\n")
        s.write("REM type build_log_" + module + ".txt\n")
        s.write("\n")
        s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
        s.write("\n")
        s.write("echo \"BUILD SUCCESSFUL\" %1\n")
        s.write("\n")
        s.write(":END\n")
        s.close()
    
        #script execution
        os.system(fileBuild)
        #script removing
        os.remove(fileBuild)
        
        os.chdir(currentPathScript)
    
def buildMingw():
    print "Starting to Build MAF with Mingw"
    module = param['module']
    suffix = 'Test'
    
    flagTest = param['enable-test']

    if(flagTest):
        dirList = os.listdir(module)
        for item in dirList:
            val =  'maf' in item
            isTest = 'Test' in item
            isADir = os.path.isdir(os.path.join(module,item))
            exists = os.path.exists(os.path.join(module,item))
            if (isTest == True and val and isADir):
                #script generation
                os.chdir(os.path.join(currentPathScript, module, item))
                fileBuild = "buildMingw_" + item + ".bat"
                s = open(fileBuild, 'w')    
    
                s.write("mingw32-make.exe clean\n")
                s.write("qmake.exe -spec win32-g++ -r\n")
        
                s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
                s.write("mingw32-make.exe\n")
                s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
                s.write("mingw32-make.exe install\n")
                s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
                s.write("\n")
                s.write("echo \"BUILD SUCCESSFUL\" %1\n")
                s.write("\n")
                s.write(":END\n")
                s.close()
    
                #script execution
                os.system(fileBuild)
                #script removing
                os.remove(fileBuild)
        
                os.chdir(currentPathScript)
    else:
        #script generation
        os.chdir(os.path.join(currentPathScript, module))
        fileBuild = "buildMingw_" + module + ".bat"
        s = open(fileBuild, 'w')    
    
        s.write("mingw32-make.exe clean\n")
        s.write("qmake.exe -spec win32-g++ -r\n")
        
        s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
        s.write("mingw32-make.exe\n")
        s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
        s.write("mingw32-make.exe install\n")
        s.write("IF %ERRORLEVEL% NEQ 0 GOTO END\n")
        s.write("\n")
        s.write("echo \"BUILD SUCCESSFUL\" %1\n")
        s.write("\n")
        s.write(":END\n")
        s.close()
    
        #script execution
        os.system(fileBuild)
        #script removing
        os.remove(fileBuild)
        
        os.chdir(currentPathScript)
     
def buildGCC4():
    print "Starting to Build MAF with GCC4"
    module = param['module']
    suffix = 'Test'
    
    flagTest = param['enable-test']
    
    if(flagTest):
        dirList = os.listdir(module)
        for item in dirList:
            val =  'maf' in item
            isTest = 'Test' in item
            isADir = os.path.isdir(os.path.join(module,item))
            exists = os.path.exists(os.path.join(module,item))
            if (isTest == True and val and isADir):
                #script generation
                os.chdir(os.path.join(currentPathScript, module, item))
                fileBuild = "buildGCC4_" + item + ".sh"
                s = open(fileBuild, 'w')    
    
                s.write("make clean\n")
                s.write("qmake CONFIG+=debug CONFIG+=maf_use_lcov;\n")
        
                s.write("make;\n")
                s.write("if [ $? != 0 ]; then\n")
                s.write(" exit 1\n")
                s.write("fi\n")
                s.write("make install\n")
                s.write("if [ $? != 0 ]; then\n")
                s.write(" exit 1\n")
                s.write("fi\n")
        
                s.write("echo \"BUILD SUCCESSFUL\" %1\n")
                s.close()
    
                #script execution
                os.system("bash " + fileBuild)
                #script removing
                os.remove(fileBuild)
        
                os.chdir(currentPathScript)
    else:
        #script generation
        os.chdir(os.path.join(currentPathScript, module))
        fileBuild = "buildGCC4_" + module + ".sh"
        s = open(fileBuild, 'w')    
    
        s.write("make clean\n")
        s.write("qmake CONFIG+=debug CONFIG+=maf_use_lcov;\n")
        
        s.write("make;\n")
        s.write("if [ $? != 0 ]; then\n")
        s.write(" exit 1\n")
        s.write("fi\n")
        s.write("make install\n")
        s.write("if [ $? != 0 ]; then\n")
        s.write(" exit 1\n")
        s.write("fi\n")
        
        s.write("echo \"BUILD SUCCESSFUL\"\n")
        s.close()
    
        #script execution
        os.system("bash " + fileBuild)
        #script removing
        os.remove(fileBuild)
        
        os.chdir(currentPathScript)

def errhandler():
   print "Unrecognized compiler"



def createBinDirectories():
    baseDir = param['directory']
    binDir = baseDir + "/bin"
    includeDir = baseDir + "/include"
    debugDir = binDir + "/Debug"
    releaseDir = binDir + "/Release"
    
    if not os.path.exists(baseDir):
     os.makedirs(baseDir)
     
    if not os.path.exists(binDir):
     os.makedirs(binDir)
     
    if not os.path.exists(includeDir):
     os.makedirs(includeDir)
     
    if not os.path.exists(debugDir):
     os.makedirs(debugDir)
     
    if not os.path.exists(releaseDir):
     os.makedirs(releaseDir)
     
    pass

def build():
    buildDir = ""
    try:
        buildDir = param['directory']
    except:
        if(param['compiler'] == 'gcc4'):
            param['directory'] = currentPathScript + "/../Install"
        else: 
            param['directory'] = currentPathScript + "\\..\\Install"
            
    #print param
    createBinDirectories()
    mafInstallModuleCreate()
    mafInstallTestCreate()

    takeBuild = {
    "vs2008": buildVS2008,
    "mingw": buildMingw,
    "gcc4": buildGCC4,
    }
    
    try:
        takeBuild.get(param['compiler'],errhandler)()
    except Exception, e:
        print "Error trying to launch the compiler." , e   

        
def mafInstallModuleCreate():
    #to remove at the beginning
    fullPath = os.path.abspath(currentPathScript  + "/mafInstallModule.pri")
    fullPathIn = os.path.abspath(currentPathScript  + "/mafInstallModule.pri.in")
    if(os.path.exists(fullPath)):
        os.remove(fullPath)
    directory = param['directory']
    f = open(fullPath, 'w')
    import re
    data = open(fullPathIn).read()
    old = '@@MAF_INSTALL_BASE@@'
    new = directory.replace('\\','\\\\')
    f.write( re.sub(old,new,data) )
    f.close() 
    
def mafInstallTestCreate():
    #to remove at the beginning
    fullPath = os.path.abspath(currentPathScript  + "/mafInstallTest.pri")
    fullPathIn = os.path.abspath(currentPathScript + "/mafInstallTest.pri.in")
    if(os.path.exists(fullPath)):
        os.remove(fullPath)
    directory = param['directory']
    f = open(fullPath, 'w')
    import re
    data = open(fullPathIn).read()
    old = '@@MAF_INSTALL_BASE@@'
    new = directory.replace('\\','\\\\')
    temp = re.sub(old,new,data)
    old = '@@MAF_SOURCE_DIR@@'
    new = currentPathScript.replace('\\','\\\\')
    f.write( re.sub(old,new,temp) )
    f.close()
        
def usage():
    print "python mafBuild.py [-m moduleName] [-c vs2008 | mingw | gcc4] [-d directory]"

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hm:c:d:t", ["help", "module=", "compiler=", "directory=", "enable-Test"])
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)

    param['enable-test'] = False
    
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-c", "--compiler"):
            param['compiler'] = a
        elif o in ("-m", "--module"):
            param['module'] = a
        elif o in ("-d", "--directory"):
            param['directory'] = os.path.normpath(a)
        elif o in ("-t", "--directory"):
            param['enable-test'] = True
        else:
            assert False, "unhandled option"
    
    if(len(param) == 0):
        usage()
        #print currentPathScript
        return
    
    
    build()

if __name__ == "__main__":
  main()