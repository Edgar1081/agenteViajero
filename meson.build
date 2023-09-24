project('Agente Viajero', 'cpp',
  version : '0.9')

sqlite_dep = dependency('sqlite3')

# Main executable
reproducefile = ['src/main/execs/AgenteViajero.cpp']
executable('reproduce', reproducefile, dependencies : [sqlite_dep], c_args : ['-g'])

#Eval an array
evalfile = ['src/main/execs/Eval.cpp']
executable('eval', evalfile, dependencies : [sqlite_dep], c_args : ['-g'])

#Prints full eval
evaltotalfile = ['src/main/execs/Evalstotal.cpp']
executable('fullevals', evaltotalfile, dependencies : [sqlite_dep], c_args : ['-g'])

#Sweep
sweepfile = ['src/main//execs/Sweep.cpp']
executable('sweep', sweepfile, dependencies : [sqlite_dep], c_args : ['-g'])
