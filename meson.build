project('Agente Viajero', 'cpp',
  version : '0.9')

sqlite_dep = dependency('sqlite3')
draw_dep = dependency('libxml-2.0', required: true)

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

#Draw
drawfile = ['src/main//execs/Draw.cpp']
executable('draw', drawfile, dependencies : [sqlite_dep, draw_dep], c_args : ['-g'])

#Script
runfile = ['src/main/execs/Run.cpp']
executable('script', runfile, dependencies : [sqlite_dep], c_args : ['-g'])

#Run
runfile = ['src/main//execs/Create_sols.cpp']
executable('run', runfile, dependencies : [sqlite_dep], c_args : ['-g'])

#Solve
solvefile = ['src/main//execs/Solve.cpp']
executable('solve', solvefile, dependencies : [sqlite_dep], c_args : ['-g'])
