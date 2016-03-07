#include "Player.hh"
#include <unordered_map>
#include <cmath>

using namespace std;



#define PLAYER_NAME AGE_Grinvax


struct PLAYER_NAME : public Player {


    static Player* factory () {
        return new PLAYER_NAME;
    }
  
  void amenazado (Starship s, Pos p, Dir& d) {
    //cerr << "amenazado" << endl;
    int j = 1;
    while (within_universe(p+SLOW_UP+Dir{0,j}) and j >= -4 and within_window(p+SLOW_UP+Dir{0,j}, round()+1)) {
      if (cell(p+SLOW_UP+Dir{0,j}).type == STARSHIP and movimiento_ok(p, DOWN)) d = DOWN;
      else if (cell(p+SLOW_UP+Dir{0,j}).type == STARSHIP and movimiento_ok(p, SLOW_DOWN)) d = SLOW_DOWN;
      --j;
    }
    j = 1;
    while (within_universe(p+SLOW_DOWN+Dir{0,j}) and j >= -4 and within_window(p+SLOW_DOWN+Dir{0,j}, round()+1)) {
      if (cell(p+SLOW_DOWN+Dir{0,j}).type == STARSHIP and movimiento_ok(p, UP)) d = UP;
      else if (cell(p+SLOW_DOWN+Dir{0,j}).type == STARSHIP and movimiento_ok(p, SLOW_UP)) d = SLOW_UP;
      --j;
    }
    j = -1;
    while (within_universe(p+Dir{0,j}) and j >= -4 and within_window(p+Dir{0,j}, round()+1)) {
      if (cell(p+Dir{0,j}).type == MISSILE and movimiento_ok(p, UP)) d = UP;
      else if (cell(p+Dir{0,j}).type == MISSILE and movimiento_ok(p, SLOW_UP)) d = SLOW_UP;
      --j;
    }
  }
  
  bool movimiento_ok (Pos p, Dir d) {
    //cerr << "mov_ok" << endl;
    Pos q = p+d;
    if (not within_window(q, round()) or not within_universe(q)) return false;
    int j;
    if (d == UP or d == SLOW_UP) {
      j = 1;
      while (within_universe(p+d+Dir{0,j}) and j >= -4 - second(d) and within_window(p+d+Dir{0,j}, round()+1)) {
        if (cell(p+d+Dir{0,j}).type == MISSILE or cell(p+d+Dir{0,j}).type == STARSHIP) return false;
        --j;
      }
      j = 1;
      while (within_universe(p+d+d+Dir{0,j}) and j >= -2 - second(d) and within_window(p+d+d+Dir{0,j}, round()+1)) {
        if (cell(p+d+d+Dir{0,j}).type == MISSILE or cell(p+d+d+Dir{0,j}).type == STARSHIP) return false;
        --j;
      }
    }
    if (d == DOWN or d == SLOW_DOWN) {
      j = 1;
      while (within_universe(p+d+Dir{0,j}) and j >= -4 - second(d) and within_window(p+d+Dir{0,j}, round()+1)) {
        if (cell(p+d+Dir{0,j}).type == MISSILE or cell(p+d+Dir{0,j}).type == STARSHIP) return false;
        --j;
      }
      j = 1;
      while (within_universe(p+d+d+Dir{0,j}) and j >= -2 - second(d) and within_window(p+d+d+Dir{0,j}, round()+1)) {
        if (cell(p+d+d+Dir{0,j}).type == MISSILE or cell(p+d+d+Dir{0,j}).type == STARSHIP) return false;
        --j;
      }
    }
    CType t = cell(q).type;
    if (t == STARSHIP) return false;
    if (t == MISSILE or t == ASTEROID) return false;
    if (d == FAST and (cell(p+DEFAULT).type == ASTEROID)) return false;
    if (d == UP and (cell(p+SLOW_UP).type==ASTEROID or cell(p+DEFAULT).type == ASTEROID)) return false;
    if (d == DOWN and (cell(p+SLOW_DOWN).type==ASTEROID or cell(p+DEFAULT).type == ASTEROID)) return false;
    if (d == FAST_UP and within_window(q, round()+1)) return movimiento_ok(p, UP);
    if (d == FAST_DOWN and within_window(q, round()+1)) return movimiento_ok(p, DOWN);
    if (within_window(q, round()+1)) return true;
    return false;
  }
  
  void move_pos(Starship s, Pos p) {
    //cerr << "move_pos" << endl;
    //cerr << "fila_bonus " << first(p) << " col_bonus " << second(p) << " fila_ship " << first(s.pos) << " col_ship " << second(s.pos) << " sid " << s.sid << endl;
    if (player_of(s.sid) == me()) {
      Dir bonus{0,0};
      if (first(p) > first(s.pos)) {
        if (movimiento_ok(s.pos, DOWN) and second(s.pos) != second(p)) {
          if (cell(s.pos+DOWN+DEFAULT).type == ASTEROID and cell(s.pos+FAST).type != ASTEROID) amenazado(s, s.pos, bonus=FAST);
          else if (cell(s.pos+DOWN+DEFAULT).type == ASTEROID and cell(s.pos+DEFAULT).type != ASTEROID) amenazado(s, s.pos, bonus=DEFAULT);
          else bonus=DOWN;
        }
        else if (movimiento_ok(s.pos, SLOW_DOWN) and second(s.pos)%number_window_columns() >= 1) bonus=SLOW_DOWN;
        else if (cell(s.pos+DEFAULT).type != ASTEROID) amenazado(s, s.pos, bonus=DEFAULT);
        else shoot(s.sid);
      }
      else if (first(p) < first(s.pos)) {
        if (movimiento_ok(s.pos, UP) and second(s.pos) != second(p)) {
          if (cell(s.pos+UP+DEFAULT).type == ASTEROID and cell(s.pos+FAST).type != ASTEROID) amenazado(s, s.pos, bonus=FAST);
          else if (cell(s.pos+UP+DEFAULT).type == ASTEROID and cell(s.pos+DEFAULT).type != ASTEROID) amenazado(s, s.pos, bonus=DEFAULT);
          else bonus = UP;
        }
        else if (movimiento_ok(s.pos, SLOW_UP) and second(s.pos)%number_window_columns() > 1) bonus = SLOW_UP;
        else if (cell(s.pos+DEFAULT).type != ASTEROID) amenazado(s, s.pos, bonus=DEFAULT);
        else shoot(s.sid);
      }
      
      else if (first(p) == first(s.pos)) {
        cerr << "r" << endl;
        if (movimiento_ok(s.pos, FAST)) {
          if (cell(s.pos+FAST+DEFAULT).type == ASTEROID and movimiento_ok(s.pos, SLOW_UP) and cell(s.pos+SLOW_UP).type != ASTEROID) bonus = SLOW_UP;
          else if (cell(s.pos+FAST+DEFAULT).type == ASTEROID and movimiento_ok(s.pos, SLOW_DOWN) and cell(s.pos+SLOW_DOWN).type != ASTEROID) bonus = SLOW_DOWN;
          else amenazado(s, s.pos, bonus=FAST);
        }
        else if (cell(s.pos+DEFAULT).type != ASTEROID) amenazado(s, s.pos, bonus=DEFAULT);
        else shoot(s.sid);
      }
      if (bonus != Dir{0,0}) move(s.sid, bonus);
    }
  }
  
  bool move_to_bonus(const Starship& s) {
    //cerr << "BONUS sid: " << s.sid << endl;
    Pos act;
    queue<complex<int>> Q;
    int i = first(s.pos);
    int j = 0;
    vector<vector<int>> enc(number_rows(), vector<int>(number_window_columns(),false));
    Q.push(complex<int>{0,0});
    enc[i][j] = true;
    while (not Q.empty()) {
      //cerr << "first " << first(Q.front()) << " second " << second(Q.front())  << " FIRST " << first(s.pos) << " SECOND " << second(s.pos) << endl;
      act = Q.front();
      i = first(s.pos) + first(act);
      j = 0 + second(act);
      //cerr << "i " << i << " j " << j << endl;
      Q.pop();
      if (within_window(s.pos+act,round())) {
        if (cell(s.pos + act).type == POINT_BONUS or cell(s.pos + act).type == MISSILE_BONUS) {
          //cerr << "BONUS" << endl;
          move_pos(s, s.pos+act);
          return true;
        }
        if (i-1 >= 0 and j >= 0 and not enc[i-1][j]) {
          Q.push(act + SLOW_UP);
          enc[i-1][j] = true;
        }
        if (i+1 < enc.size() and j >= 0 and not enc[i+1][j]) {
          enc[i+1][j] = true;
          Q.push(act + SLOW_DOWN);
        }
        if (i >= 0 and j < enc[0].size() and not enc[i][j+1]){
          Q.push(act + DEFAULT);
          enc[i][j+1] = true;
        }
      }
      //else cerr << "NOP" << endl;
    }
    //cerr << "NO BONUS" << endl;
    return false;
  }
  
  //Devuelve cierto si se ha disparado contra un enemigo
  bool dispara_misiles (Starship& s) {
    //cerr << "MISILES sid: " << s.sid << endl;
    int j = 1;
    Cell ahead;
    while (within_window(s.pos + complex<int>{0,j}, round()+1) and j <= 6) {
      complex<int> i (0, j);
      if (cell(s.pos+i).type == MISSILE) return false;
      if ((ahead=cell(s.pos + i)).type == STARSHIP) {
        Player_Id p = player_of(ahead.sid);
        if (p != me() and s.nb_miss > 0) {
          if (player_of(s.sid) == me()) {
            shoot(s.sid);
            return true;
          }
        }
        else return false;
      }
     ++j;
    }
    return false;
  }

  virtual void play () {
    for (Starship_Id sid = begin(me()); sid != end(me()); ++sid) {
      Starship s = starship(sid);
      bool done = false;
      if (s.alive) {
        if (not done) done = dispara_misiles(s);
        if (not done) done = move_to_bonus(s);
      }
     }
  }

};



RegisterPlayer(PLAYER_NAME);
