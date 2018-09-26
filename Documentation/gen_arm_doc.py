#!/usr/bin/env python3

import os, re, subprocess

spp = 512

def extract_doc(file_path):
    with open(file_path) as f:
        print('Processing ' + str(file_path))

        doc_sections = []
        curr_section = []
        
        in_doc = False
        
        for l in f.readlines():
            line = l.strip()
            
            if not in_doc:
                match = re.match(r'^/\*\* ?(.*)$', line)

                if match != None:
                    line = match.group(1).replace('%', '\%')
                    curr_section.append(line)
                    in_doc = True
            else:
                match = re.match(r'(.*)\*/$', line)

                if match != None:
                    in_doc = False
                    line = match.group(1).replace('%', '\%')
                    curr_section.append(line)
                    doc_sections.append(curr_section)
                    curr_section = []
                else:
                    #line = line[1:].strip() if line.startswith('*') else line
                    line = line.replace('%', '\%')
                    curr_section.append(line)

    return doc_sections

def process_header(header):
    processed_header = {
        'file'       : '',
        'name'       : '',
        'type'       : '',
        'description': '',
        'artist'     : '',
    }

    in_verbatim = False

    for line in header:
        lstrip = line[1:].strip() if line.startswith('*') else line
        lstrip2 = line[1:] if line.startswith('*') else line
        
        if lstrip.startswith('@ignore'):
            pass
        elif lstrip.startswith('@file'):
            pass

        # @type: Set the type that is going to be documented
        elif lstrip.startswith('@type'):
            type_name = lstrip[len('@type'):].strip()
            processed_header['type'] = type_name

        # @brief: Chapter title
        elif lstrip.startswith('@brief'):
            processed_header['name'] = lstrip[len('@brief'):].strip()

        # @artist: Redering illustrations
        elif lstrip.startswith('@artist'):
            processed_header['artist'] = lstrip[len('@artist'):].strip()

        # The description part
        elif lstrip.startswith('\\begin{verbatim}'):
            if not lstrip.endswith('\\end{verbatim}'):
                in_verbatim = True
                processed_header['description'] += '\n\\verb?' + lstrip[len('\\begin{verbatim}'):] + '?\\newline\n'
            else:
                processed_header['description'] += '\n\\verb?' + lstrip[len('\\begin{verbatim}'):-len('\\end{verbatim}')] + '?\\newline\n'

        elif lstrip2.endswith('\\end{verbatim}'):
            processed_header['description'] += '\\verb?' + lstrip2[:-len('\\end{verbatim}')] + '?\n'
            in_verbatim = False

        else:
            if in_verbatim:
                processed_header['description'] += '\\verb?' + lstrip2 + '?\\newline\n'
            else:
                processed_header['description'] += lstrip + '\n'

    return processed_header

def process_doc(doc_sections):
    documented_types = []

    processed_doc = {
        'header'     : {},
        'have_header': False,
        'sections'   : [],
    }

    in_main_section2 = True
    curr_level = 0

    for section in doc_sections:
        doc_struct = {
            'name'       : '',
            'type'       : '',
            'main_desc'  : '',
            'subsections': [],
            'command'    : '',
            'description': '',
            'artist'     : [],
            'params'     : [],
            'level'      : curr_level,
        }
        
        subsection_struct = {
            'name'       : '',
            'type'       : '',
            'main_desc'  : '',
            'subsections': [],
            'command'    : '',
            'description': '',
            'artist'     : [],
            'params'     : [],
            'level'      : curr_level + 1,
        }
        

        in_verbatim = False
        in_main_section = in_main_section2

        for line in section:
            lstrip = line[1:].strip() if line.startswith('*') else line
            lstrip2 = line[1:] if line.startswith('*') else line
            
            # @file An header must start with this keyword and is going to be
            # processed differently
            if lstrip.startswith('@file'):
                processed_doc['header'] = process_header(section)
                processed_doc['have_header'] = True
                if processed_doc['header']['type']:
                    documented_types.append(processed_doc['header']['type'])
                break

            # @ignore: Ignore what is following
            if lstrip.startswith('@ignore'):
                pass
            
            # @pushsection: Add a subsection to the current section
            elif lstrip.startswith('@pushsection'):
                section_name = lstrip[len('@pushsection'):].strip()
                doc_struct['name'] = section_name
                curr_level += 1

            # @popsection: Get out of the current section
            elif lstrip.startswith('@popsection'):
                curr_level -= 1
                break

            # @section: Main level section
            elif lstrip.startswith('@section'):
                section_name = lstrip[len('@section'):].strip()
                doc_struct['name'] = section_name
                doc_struct['level'] = 0
                curr_level = 1

            # @type: Set the type that is going to be documented
            elif lstrip.startswith('@type'):
                type_name = lstrip[len('@type'):].strip()
                documented_types.append(type_name)
                
                if in_main_section:
                    doc_struct['type'] = type_name
                else:
                    doc_struct['subsections'][-1]['type'] = type_name

            # @brief: Block section title
            elif lstrip.startswith('@brief'):
                section_name = lstrip[len('@brief'):].strip()
                if in_main_section:
                    doc_struct['name'] = section_name
                else:
                    doc_struct['subsections'][-1]['name'] = section_name

            # @def: Set the define or function to document
            elif lstrip.startswith('@def'):
                in_main_section = False
                commad = lstrip[len('@def'):].strip()
                # Creates a subsection
                doc_struct['subsections'].append({
                    'name'       : '',
                    'type'       : '',
                    'main_desc'  : '',
                    'subsections': [],
                    'command'    : '',
                    'description': '',
                    'artist'     : [],
                    'params'     : [],
                    'level'      : curr_level + 1,
                })
                doc_struct['subsections'][-1]['command'] = commad;

            # @fdef: Following def: Add a line to the current @def
            elif lstrip.startswith('@fdef'):
                commad = lstrip[len('@fdef'):]
                doc_struct['subsections'][-1]['command'] +='\n' + commad
                
            # @artist: Redering illustrations
            elif lstrip.startswith('@artist'):
                artist_args = lstrip[len('@artist'):].strip()
                if in_main_section:
                    doc_struct['artist'].append(artist_args)
                else:
                    doc_struct['subsections'][-1]['artist'].append(artist_args)

            # @param: set a parameter with
            # @param param_name <space(s)> param_type <space(s)> param_description
            elif lstrip.startswith('@param'):
                lstrip = lstrip[len('@param'):].strip()
                param = ' '.join(lstrip.split()).split(' ', 2)
                
                if in_main_section:
                    doc_struct['params'].append(param)
                else:
                    doc_struct['subsections'][-1]['params'].append(param)
                    
            # The description part
            elif lstrip.startswith('\\begin{verbatim}'):
                if not lstrip.endswith('\\end{verbatim}'):
                    in_verbatim = True
                    description = '\n\\verb?' + lstrip[len('\\begin{verbatim}'):] + '?\\newline\n'
                else:
                    description = '\n\\verb?' + lstrip[len('\\begin{verbatim}'):-len('\\end{verbatim}')] + '?\\newline\n'

                if in_main_section:
                    doc_struct['description'] += description
                else:
                    doc_struct['subsections'][-1]['description'] += description

            elif lstrip2.endswith('\\end{verbatim}'):
                description = '\\verb?' + lstrip2[:-len('\\end{verbatim}')] + '?\n'
                in_verbatim = False

                if in_main_section:
                    doc_struct['description'] += description
                else:
                    doc_struct['subsections'][-1]['description'] += description
            else:
                if in_verbatim:
                    description = '\\verb?' + lstrip2 + '?\\newline\n'
                else:
                    description = lstrip + '\n'
                    
                if in_main_section:
                    doc_struct['description'] += description
                else:
                    doc_struct['subsections'][-1]['description'] += description
        else:
            # Adds to sections only if did not encountered a break\
            # i.e. if no header was encountered
            processed_doc['sections'].append(doc_struct)
        
    return processed_doc, documented_types

def artist(args_str, scene_dir, output_file_noext, latex_dir):    
    output_path = os.path.join(os.path.abspath(latex_dir), output_file_noext)
    
    if not os.path.exists(output_path + '.png'):
        args = ['artist'] + (' '.join(args_str.split())).split() + ['-b', '-o', output_path]
        args += ['-DSAMPLES=' + str(spp)]
        subprocess.run(args, cwd=scene_dir)
        subprocess.run(['convert',
                        output_path + '.tiff',
                        output_path + '.png'],
                        cwd=scene_dir)
    
    return output_file_noext + '.png'

def process_latex_section(section, scene_dir, latex_dir, types_ref, level = 0):
    level = section['level']
    subsection_t = ''
    if level == 0:
        subsection_t = '\\section{{{}}}\n'
    elif level == 1:
        subsection_t = '\\subsection{{{}}}\n'
    else:
        subsection_t = '\\subsubsection{{{}}}\n'

    ref_t = '\\hyperref[{}]{{{}}}\n'
    label_t = '\\label{{{}}}\n'
    
    latex_str = ''

    if section['name']:
        latex_str += subsection_t.format(section['name'])
        latex_str += '\n\n'
    
    if section['type']:
        latex_str += label_t.format(section['type'])

    if section['command']:
        latex_str += '\\noindent\\begin{verbatim}' + section['command'] + '\\end{verbatim}\n'
        #if section['params']:
            #latex_str += '\n\\vspace{0.1cm}\n'

    desc = section['description'].strip()

    # Put the artist rendering next to the description only if there is a single
    # rendering
    if len(section['artist']) == 1:
        latex_str += '\\noindent\\begin{tabularx}{\\textwidth}{@{}Xr}\n'
        latex_str += desc
        latex_str +=  '& \n'
        
        preview_name_noext = section['name'].replace(' ', '-')
        preview_name = artist(section['artist'][0], scene_dir, preview_name_noext, latex_dir)
        latex_str += '\\raisebox{-\\totalheight}{\\includegraphics[scale=0.5]{' + preview_name + '}}'
        latex_str += '\\\\\n'
        latex_str += '\\end{tabularx}\n'
    else:
        latex_str += desc + '\n\n'

    # Else, append all the rending after the description
    if len(section['artist']) > 1:
        latex_str += '\n\n\\begin{table}[h]\n\\centering\n'
        latex_str += '\\begin{tabular}{' + (len(section['artist']) * 'c') + '}\n'
        
        for artist_cmd, idx_artist in zip(section['artist'], range(len(section['artist']))):
            preview_name_noext = section['name'].replace(' ', '-') + str(idx_artist)
            preview_name = artist(artist_cmd, scene_dir, preview_name_noext, latex_dir)
            
            if idx_artist > 0:
                latex_str += ' & '
            latex_str += '\\includegraphics[scale=0.5]{' + preview_name + '}'
            
        latex_str += '\\\\\n\\end{tabular}\n\\end{table}\n\n'

    # Parameter list / description
    param_line_t = '\\textbf{{\\verb?{}?}} & {} & {} \\\\\n'
    if section['params']:
        #latex_str += '\n\\vspace{0.1cm}\n'
        latex_str += '\\noindent\\begin{tabularx}{\\textwidth}{ccX}\n'
        latex_str += '\\toprule\n'
        latex_str += '\\textbf{Parameter} & \\textbf{Type} & \\textbf{Description}\\\\\n'
        latex_str += '\\midrule\n'

        for p in section['params']:
            param_verb = '\\verb?{}?'.format(p[1])
            param_type = ref_t.format(p[1], param_verb) if p[1] in types_ref else param_verb
            latex_str += param_line_t.format(p[0], param_type, p[2])

        latex_str += '\\bottomrule\n'
        latex_str += '\\end{tabularx}\\\n'
    #latex_str += '\n\\vspace{0.2cm}\n'

    if section['subsections']:
        for s in section['subsections']:
            latex_str += '\n\n'
            latex_str += '\n\\vspace{0.5cm}\n'
            latex_str += process_latex_section(s, scene_dir, latex_dir, types_ref, level + 1)

    return latex_str

def gen_latex_section(processed_file, section_name, scene_dir, latex_dir, types_ref):
    latex_str = ''

    latex_section = '\\chapter{{{}}}\n'
    if processed_file['have_header']:
        latex_str += latex_section.format(processed_file['header']['name'])
        latex_str += processed_file['header']['description']
    else:
        latex_str += latex_section.format(section_name)
    
    if processed_file['have_header'] and processed_file['header']['type']:
        latex_str += '\\label{' + processed_file['header']['type'] + '}\n'

    for s in processed_file['sections']:
        latex_str += process_latex_section(s, scene_dir, latex_dir, types_ref)
        
    return latex_str

def main():
    # Set the place where the files should be parsed to look for documentation
    ARM_Interface_src_dir = os.path.join('..',
                                         'Source',
                                         'ClassLibraries','ARM_Interface')

    # Set which file should be parsed and which name will be used for the
    # documentation
    files_to_process = [
        ('Action Sequence'       , 'ARM_ActionSequence.h'                    ),
        ('Actions'               , 'ARM_Action.h'                            ),
        ('Image Samplers'        , 'ARM_ImageSampler.h'                      ),
        ('Ray casters'           , 'ARM_RayCasting.h'                        ),
        ('Path Tracers'          , 'ARM_PathspaceIntegrator.h'               ),
        ('Cameras'               , 'ARM_Camera.h'                            ),
        ('Shapes'                , 'ARM_Shape.h'                             ),
        ('Transformations'       , 'ARM_Trafo.h'                             ),
        ('Spectra'               , 'ARM_Colour.h'                            ),
        ('Sky Models'            , 'ARM_SkyModel.h'                          ),
        ('Surface Materials'     , 'ARM_SurfaceMaterial.h'                   ),
        ('Volume Materials'      , 'ARM_VolumeMaterial.h'                    ),
        ('Environment Materials' , 'ARM_EnvironmentMaterial.h'               ),
        ('Volumes'               , 'ARM_VolumeData.h'                        ),
        ('Foundation'            , 'ARM_Foundation.h'                        ),
        ('Scene graph'           , 'ARM_Scenegraph.h'                        ),
        ('Node CSG Operators'    , 'ArNode_ARM_CSG_Operators.h'              ),
        ('Node attributes'       , 'ArNode_ARM_SpecificAttributes.h'         ),
    ]

    # Extract the types

    # This comes into place when the documentation ask for a specific scene
    # to render using the @artist flag
    example_scene_dir = os.path.join('.', 'ARM_Doc_Scene')

    output_dir = 'generated'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    documented_sections = []
    documented_types = []

    for section_name, filename in files_to_process:
        # Extract from the file the documentation part in the /** ... */
        # sections
        sections = extract_doc(os.path.join(ARM_Interface_src_dir, filename))

        # Do some processing from the extracted sections to isolate tags
        section_processed, type_processed = process_doc(sections)
        documented_types += type_processed
        documented_sections.append(section_processed)

    latex_str = ''

    for (section_name, filename), sections in zip(files_to_process, documented_sections):
        # Format the tags to output a LaTeX document
        latex_str += gen_latex_section(
            sections,
            section_name,
            example_scene_dir,
            output_dir,
            documented_types)

    # Write the LaTeX document
    with open(os.path.join(output_dir, 'ARM_Interface_gen.tex'), 'w') as f:
        f.write(latex_str)

    # Run LaTeX on the main.tex which includes the processed document
    # ARM_Interface_gen.tex
    subprocess.run(['pdflatex', 'ARM_Interface'])
    subprocess.run(['pdflatex', 'ARM_Interface'])

if __name__ == '__main__':
    main()
