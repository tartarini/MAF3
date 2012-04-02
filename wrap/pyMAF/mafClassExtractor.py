import re

class mafClassExtractor():

    def __init__(self):
        """ empty init """

        # regular expressions setup
        self.not_operator = re.compile('^[a-zA-Z0-9_~]+$')


    def extractFromDoxygen(self, doxygen ):
        """ extract class details from doxygen doxygen

            Returns:

                a dictionary
        """

        classDetails = {'namespace':'',
                        'compoundname':'',
                        'location':'',
                        'public_destructor':False,
                        'destructor_method':'release',
                        'property':[],
                        'protected_attrib':[],
                        'private_attrib':[],
                        'public_attrib':[],
                        'public_func':[],
                        'private_func':[],
                        'signal':[],
                        'public_slot':[],
                        'protected_func':[],
                        'has_properties': False,
                        'export_macro':''
                        }

        compoundname = doxygen.find("./compounddef/compoundname", namespaces=doxygen.nsmap)

        # class general attributes
        if compoundname.text.count("::"):
            classDetails['namespace'], classDetails['compoundname' ] = compoundname.text.split("::")
        else:
            classDetails['compoundname' ] = compoundname.text

        classDetails['export_macro'] = classDetails['namespace'].upper() + 'SHARED_EXPORT'

        location = doxygen.find("./compounddef/location", namespaces=doxygen.nsmap)
        if location is not None:
            classDetails['location'] = location.attrib["file"]

        # check if the class inherites from qobject and has to provide properties setter, getter
        for node in doxygen.iterfind('./compounddef/inheritancegraph/node'):
            if node[0].text.count('mafReferenceCounted'):
                classDetails['has_properties'] = True
                break


        # methods attributes
        for key in ['public_func']:
            for sectiondef in doxygen.iterfind('./compounddef/sectiondef[@kind="%s"]' % key.replace("_","-"), namespaces=doxygen.nsmap):
                for memberdef in sectiondef.getchildren():
                    element = {'param':[], 'operator':False, 'destructor': False, 'macro':False}

                    for attr in memberdef.attrib.keys():
                        element[attr] = memberdef.attrib[attr]

                    for memberdef_child in memberdef.getchildren():
                        if memberdef_child.tag == 'param':
                            param = {}
                            for memberdef_grandchild in memberdef_child.getchildren():
                                if len(memberdef_grandchild.getchildren()):
                                    param[ memberdef_grandchild.tag ] = memberdef_grandchild[0].text

                                    #TODO a better check here for all pointer typedef
                                    if not memberdef_grandchild[0].text.lower().count('pointer'):
                                        param[ memberdef_grandchild.tag ] += '*'
                                else:
                                    param[ memberdef_grandchild.tag ] = memberdef_grandchild.text
                            element['param'].append(param)

                        elif memberdef_child.tag in ['briefdescription']:
                            if len(memberdef_child.getchildren()):
                               element[ memberdef_child.tag ] = memberdef_child[0].text

                        elif memberdef_child.tag in ['detaileddescription']:
                            if len(memberdef_child.getchildren()):
                                element[ memberdef_child.tag ] = memberdef_child[0].text

                        elif memberdef_child.tag in ['name']:
                            element[ memberdef_child.tag ] = memberdef_child.text

                            if not re.match(self.not_operator, memberdef_child.text):
                                element['operator'] = True
                            if memberdef_child.text.count("~"):
                                element['destructor'] = True
                                if classDetails['compoundname'].startswith('vtk'):
                                    classDetails['destructor_method'] = 'Delete'

                            if memberdef_child.text.lower().count('macro'):
                                element['macro'] = True



                        else:
                            element[ memberdef_child.tag ] = memberdef_child.text

                    classDetails[key].append(element)

        return classDetails



