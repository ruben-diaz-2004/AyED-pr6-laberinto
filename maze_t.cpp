// AUTOR: Rubén Díaz Marrero
// FECHA: 01/05/2023
// EMAIL: alu0101552613@ull.edu.es
// VERSION: 1.0
// ASIGNATURA: Algoritmos y Estructuras de Datos
// PRÁCTICA Nº: 6
// COMENTARIOS: se indican entre [] las pautas de estilo aplicadas de
//              "C++ Programming Style Guidelines"
//              https://geosoft.no/development/cppstyle.html

#include "maze_t.hpp"


// constructor
maze_t::maze_t():
matrix_(),
visited_(), 
i_start_(-1),
j_start_(-1), 
i_end_(-1),
j_end_(-1)
{}


// destructor
maze_t::~maze_t()
{}



// método que resuelve el laberinto invocando al método recursivo
bool
maze_t::solve(dll_t<pair_t<int>>& salida, int& cuentapasos)
{
  return solve_(i_start_, j_start_, salida, cuentapasos);
}



// lee el laberinto en forma de matriz desde la entrada
istream&
maze_t::read(istream& is)
{ 
  int m, n;
  is >> m >> n;
  assert(m > 0 && n > 0);
  
  matrix_.resize(m, n);
  visited_.resize(m, n);
  
  for (int i = 1; i <= m; i++)
  {
    for (int j = 1; j <= n; j++)
    {
      short s;
      is >> s;
      
      // si el dato es un START_ID -> entrada al laberinto
      if (s == START_ID)
      {
        i_start_ = i;
        j_start_ = j;
      }
      // si el dato es un END_ID -> salida al laberinto
      else if (s == END_ID)
      {
        i_end_ = i;
        j_end_   = j;
      }
      
      matrix_(i, j) = s;
    }
  }

  // comprobamos que se han leído correctamente la entrada y la salida  
  assert (i_start_ != -1 && j_start_ != -1 && i_end_ != -1 && j_end_ != -1);

  return is;
}



// muestra el laberinto por pantalla
ostream&
maze_t::write(ostream& os) const
{
  os << matrix_.get_m() << "x" << matrix_.get_n() << endl;
  for (int i = 1; i <= matrix_.get_m(); i++)
  {
    for (int j = 1; j <= matrix_.get_n(); j++)
      switch (matrix_(i, j))
      { 
        case START_ID: os << START_CHR; break;
        case END_ID:   os << END_CHR;   break;
        case WALL_ID:  os << WALL_CHR;  break;
        case PASS_ID:  os << PASS_CHR;  break;
        case PATH_ID:  os << PATH_CHR;  break;
      }
    os << endl;
  }
  
  return os; 
}



// FASE I
// comprueba que la fila i y columna j son válidas antes de pasar a ellas
bool
maze_t::is_ok_(const int i, const int j) const 
{
  // retornar true si se cumplen TODAS estas condiciones:
  // - fila i y la columna j están dentro de los límites del laberinto,
  // - la celda en (i, j) no puede ser un muro,
  // - la celda (i, j) no puede haber sido visitada antes.
  if ((i > 0 && i <= matrix_.get_m()) && (j > 0 && j <= matrix_.get_n()) && 
  (matrix_(i, j) != 1) && (visited_(i,j) == false)) return true;

  return false;
}

// FASE II y FASE III
// método recursivo que resuelve el laberinto
bool
maze_t::solve_(const int i, const int j, dll_t<pair_t<int>>& salida, int& cuentapasos)
{
  // CASO BASE:
  // retornar 'true' si 'i' y 'j' han llegado a la salida
  if (i == i_end_ && j == j_end_) return true;
  // [poner código aquí]

  // marcamos la celda como visitada
  visited_(i, j) = true;
  
  // CASO GENERAL:
  // para cada una de las 4 posibles direcciones (N, E, S, W) ver si es posible
  for (int k{0}; k < 4; ++k) {
    if (is_ok_(i + i_d[k], j + j_d[k])) {
      if (solve_(i + i_d[k], j + j_d[k], salida, cuentapasos) == true) {
        pair_t<int>* dato = new pair_t<int>(i + i_d[k], j + j_d[k]);
        dll_node_t<pair_t<int>>* puntero_nodo = new dll_node_t<pair_t<int>>(* dato);
        salida.push_front(puntero_nodo);
        if (i_d[k] == 1 && j_d[k] == 0) cuentapasos++;
      //  salida.push_front( new dll_node_t<pair_t<int>>(*(dato)));
        matrix_(i + i_d[k], j + j_d[k]) = PATH_ID;
        return true;
      }
    }
  }
  // el desplazamiento (is_ok_) y, en ese caso, intentar resolver el laberinto
  // llamando recursivamente a 'solve'. 
  // Si la llamada devuelve 'true', poner en la celda el valor PATH_ID, y
  // propagarla retornando también 'true'

  // [poner código aquí]
  
  // desmarcamos la celda como visitada (denominado "backtracking") y
  // retornamos 'false'
  visited_(i, j) = false;
  return false;
}



// operador de entrada sobrecargado
istream&
operator>>(istream& is, maze_t& M)
{
  return M.read(is);
}



// operador de salida sobrecargado
ostream&
operator<<(ostream& os, const maze_t& M)
{
  return M.write(os);
}
