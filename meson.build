project('Agente Viajero', 'cpp',
  version : '0.1')

sqlite_dep = dependency('sqlite3')

# Main executable
agente_sources = ['src/main/AgenteViajero.cpp']
executable('solve', agente_sources, dependencies : [sqlite_dep], c_args : ['-g'] )
