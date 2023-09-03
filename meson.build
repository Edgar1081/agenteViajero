project('Agente Viajero', 'cpp',
  version : '0.1',
  default_options : ['optimization=plain'])

sqlite_dep = dependency('sqlite3')

# Main executable
agente_sources = ['src/main/AgenteViajero.cpp']
executable('solve', agente_sources, dependencies : [sqlite_dep])
