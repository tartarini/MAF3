#script for build on different platforms with different compilers maf.
#Using this prevent from using cmake. (support compilers are gcc on linux and mac, mingw and visual studio 2008 in windows)

import os, sys
import getopt
import shutil

currentPathScript = os.path.split(os.path.realpath(__file__))[0]
param = {}
paramError = {}

def buildVS2008():
    print "Starting to Build MAF with VS2008"
    module = param['module']
    suffix = 'Test'
    
    flagTest = param['test']

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
    
    flagTest = param['test']

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
    
    flagTest = param['test']
    
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
    binDir = os.path.join(baseDir,"bin")
    includeDir = os.path.join(baseDir, "include")
    debugDir = os.path.join(binDir, "Debug")
    releaseDir = os.path.join(binDir, "Release")
    
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
        param['directory'] = os.path.join(currentPathScript, ".." , "Install")
            
    #print param
    createBinDirectories()
    mafCommonCreate()
    #mafInstallModuleCreate()
    #mafInstallTestCreate()

    takeBuild = {
    "vs2008": buildVS2008,
    "mingw": buildMingw,
    "gcc4": buildGCC4,
    }
    
    try:
        takeBuild.get(param['compiler'],errhandler)()
    except Exception, e:
        print "Error trying to launch the compiler." , e
        
def generateFromCmake():
    #create build dir
    buildDir = os.pathjoin("..", ".." , "MAF.build")
    if not os.path.exists(buildDir):
        os.makedirs(buildDir)
    os.chdir(buildDir)
    
    srcDir = os.path(currentPathScript, "..")
    #cmake project
    command = "cmake " + srcDir
    os.system(command)
    os.chdir(currentPathScript)
    
def mafCommonCreate():
    #to remove at the beginning
    fullPath = os.path.abspath(currentPathScript  + "/mafCommon.pri")
    fullPathIn = os.path.abspath(currentPathScript  + "/mafCommon.pri.in")
    if(os.path.exists(fullPath)):
        os.remove(fullPath)
    directory = param['directory']
    print directory
    f = open(fullPath, 'w')
    import re
    data = open(fullPathIn).read()
    old = '@@MAF_INSTALL_BASE@@'
    new = directory.replace('\\','\\\\')
    f.write( re.sub(old,new,data) )
    f.close() 

        
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
    
def removeDocDirectories():
    baseDir = param['directory']
    docDir = os.path.join(baseDir,"Doc")
    docWithTestsDir = os.path.join(baseDir, "DocWithTests")
    #qaResultDir = os.path.join(baseDir, "QAResult")
    
    if os.path.exists(docDir):
        shutil.rmtree(docDir)
     
    if os.path.exists(docWithTestsDir):
        shutil.rmtree(docWithTestsDir)
     
    #if os.path.exists(qaResultDir):
    #    os.removedirs(qaResultDir)
      
    pass
    
    
def generateDoc():
    removeDocDirectories()
    commandDoc = "\"" + os.path.join(param['doxygenPath'], "doxygen") + "\" " + "MAF3Doxyfile"
    os.system(commandDoc)
    commandDocWithTests = "\"" + os.path.join(param['doxygenPath'], "doxygen") + "\" " + "MAF3DoxyfileWithTests"
    os.system(commandDocWithTests)
    print "DOXY SUCCESSFUL"
        
def usage():
    print "python mafConfigureBuild.py [-h] [-p code|doc] [-m moduleName] [-c vs2008 | mingw | gcc4] [-d directory] [-t] [-x doxygenPath]"
    print "-h, --help                 show help (this)"
    print "-p, --process=             select modality of build, compile code or build documentation"
    print "-C, --cmake-generate       generate project with cmake"
    print "-m, --module=              select module to compile (used only if process is code)"
    print "-c, --compiler=            select the compiler (used only if process is code)"
    print "-d, --directory=           select output directory"
    print "-t, --test                 select if compile test of the selected module (used only if process is code)"
    print "-x, --doxygen-path         set where the doxygen binary is present (used only if process is doc)"
    print 
    
def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hp:Cm:c:d:tx:", ["help", "process=", "cmake-generate", "module=", "compiler=", "directory=", "test", "doxygen-path="])
        #complete check of parameters needed    
        if(len(opts) == 0):
            usage()
            return
    
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    
    #init
    param['test'] = False
    param['process'] = "code" #other option is doc
    param['cmake-generate'] = False
    
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-p", "--process"):
            acceptance = ["doc","code"]
            if(a in acceptance):
                param['process'] = a
            else:
                paramError['process'] = a
        elif o in ("-C", "--cmake-generate"):
            param['cmake-generate'] = True
        elif o in ("-c", "--compiler"):
            acceptance = ["vs2008","mingw","gcc4"]
            if(a in acceptance):
                param['compiler'] = a
            else:
                paramError['compiler'] = a
        elif o in ("-m", "--module"):
            acceptance = ["mafCore","mafQA","mafEventBus","mafTimeSpace","mafSerialization", "mafResources", "mafPluginVTK","mafApplicationLogic","mafGUI"]
            if(a in acceptance):
                param['module'] = a
            else:
                paramError['module'] = a
        elif o in ("-d", "--directory"):
            param['directory'] = os.path.normpath(a)
        elif o in ("-t", "--test"):
            param['test'] = True
        elif o in ("-x", "--doxygen-path"):
            param['doxygenPath'] = os.path.normpath(a)
        else:
            assert False, "unhandled option"
            usage()
            exit()

    if(len(paramError) != 0 ):
        for k, v in paramError.iteritems():
            print v, " is not a valid " , k
        usage()
        exit()
    
    if(param['cmake-generate'] == True):
        generateFromCmake()
        return
    
    if(param['process'] == "code"):
         build()
    elif (param['process'] == "doc"):
         generateDoc()

if __name__ == "__main__":
  main()