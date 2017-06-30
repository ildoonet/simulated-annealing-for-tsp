from distutils.core import setup, Extension
from distutils.command.build import build
from distutils.command.build_ext import build_ext


class CustomBuild(build):
    sub_commands = [
        ('build_ext', build.has_ext_modules),
        ('build_py', build.has_pure_modules),
        ('build_clib', build.has_c_libraries),
        ('build_scripts', build.has_scripts),
    ]


class CustomBuildExtCommand(build_ext):
    """build_ext command for use when numpy headers are needed."""
    def run(self):

        # Import numpy here, only when headers are needed
        import numpy

        # Add numpy headers to include_dirs
        self.include_dirs.append(numpy.get_include())

        # Call original build_ext command
        build_ext.run(self)


pytspsa = Extension('_pytspsa',
                    sources=[
                        # 'tsp_log.h',
                        # 'tsp_sa.h',
                        'tsp_log.cpp',
                        'tsp_sa.cpp',
                        'tsp_sa_solution.cpp',
                        'tsp_sa.i',
                    ],
                    swig_opts=[
                        '-c++',
                        # '-py3'
                    ],
                    extra_compile_args=['-std=c++11'],
                    include_dirs=['./'],
                    # runtime_library_dirs=[],
                    # libraries=[],
                    )

if __name__ == '__main__':
    setup(name='pytspsa',
          version='0.1.14',
          author='ildoonet',
          author_email='ildoo@ildoo.net',
          platforms=['x86_64'],
          cmdclass={
              'build': CustomBuild,
              'build_ext': CustomBuildExtCommand
          },
          data_files=[('modules', ['modules/numpy.i']),
                      ('', ['tsp_sa.h', 'tsp_log.h', 'Clock.h'])],
          py_modules=['pytspsa'],
          ext_modules=[pytspsa],
          description="""
          This package is for solving traveling salesman problem by using simulated annealing meta heuristic. 

$ pip install pytspsa
$ python

``` 
import numpy 
import pytspsa 


solver = pytspsa.Tsp_sa() 
c = [ 
[0, 0], 
[0, 1], 
[0, 2], 
[0, 3] 
] 
c = numpy.asarray(c, dtype=numpy.float32) 
solver.set_num_nodes(4) 
solver.add_by_coordinates(c) 
solver.set_t_v_factor(4.0) 

# solver.sa() or sa_auto_parameter() will solve the problem. 
solver.sa_auto_parameter(12) 

# getting result 
solution = solver.getBestSolution() 

print('Length={}'.format(solution.getlength())) 
print('Path= {}'.format(solution.getRoute())) 
``` 

See github page.  
          """
          # install_requires=['numpy']
          )
