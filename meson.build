project('Agente Viajero', 'cpp',
  version : '0.1')

sqlite_dep = dependency('sqlite3')

# Main executable
reproducefile = ['src/main/AgenteViajero.cpp']
executable('reproduce', reproducefile, dependencies : [sqlite_dep], c_args : ['-g'])

#Eval an array
#evalfile = ['src/main/eval.cpp']
#executable('eval', evalfile, dependencies : [sqlite_dep], c_args : ['-g'])

#Sweep
#sweepfile = ['src/main/eval.cpp']
#executable('sweep', sweepfile, dependencies : [sqlite_dep], c_args : ['-g'])
