// Project Identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
// EECS 281: Project 1
// don't use pointers
// 3d vectors 

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

enum class routeMode {
  kNone = 0,
  kStack,
  kQueue
}; 

enum class outMode {
  kNone = 0,
  kMap,
  kList
}; 

struct routeOptions {
  routeMode mode = routeMode::kNone;
};  // Options{}

struct outOptions {
  outMode mode = outMode::kMap;
};  // Options{}

void printHelp(char *argv[]) {
  cout << "Usage: " << argv[0] << " [-s --stack|-q --queue] [--output M|L] < infile > outfile | -h\n";
  cout << "This program is to help you escape the ship.\n";
}  // printHelp()

// Process the command line; there is no return value, but the Options
// struct is passed by reference and is modified by this function to send
// information back to the calling function.
void getMode(int argc, char * argv[], routeOptions &routeoptions, outOptions &outoptions) {
  // These are used with getopt_long()
  opterr = false; // Let us handle all error output for command line options
  int choice;
  int index = 0;
  option long_options[] = {
    // Fill in two lines, for the "mode" ('m') and
    // the "help" ('h') options.
    { "stack",  no_argument,      nullptr,  's' },
    { "queue",  no_argument,      nullptr,  'q' },
    { "output", required_argument,nullptr,  'o' },
    { "help",   no_argument,      nullptr,  'h' },
    { nullptr, 0,                 nullptr,  '\0'},
  };  // long_options[]
  // Fill in the double quotes, to match the mode and help options.
  while ((choice = getopt_long(argc, argv, "sqo:h", long_options, &index)) != -1) {
    switch (choice) {
      case 'h':
        printHelp(argv);
        exit(0);
      case 's':
        if (routeoptions.mode==routeMode::kNone){
          routeoptions.mode = routeMode::kStack;
          break;
        }
        else{
          cerr << "Error: Multiple routing modes specified" <<endl;
          exit(1);
        }
      case 'q':
        if (routeoptions.mode==routeMode::kNone){
          routeoptions.mode = routeMode::kQueue;
          break;
        }
        else{
          cerr << "Error: Multiple routing modes specified" <<endl;
          exit(1);
        }
      case 'o':{
        string arg{optarg};
        if (arg != "L" && arg != "M") {
          cerr << "Error: invalid mode" << endl;
          cerr << "  I don't recognize: " << arg << endl;
          exit(1);
        }  
        if (arg == "M") {
          outoptions.mode = outMode::kMap;
        } else {
          outoptions.mode = outMode::kList;
        }  // if ..arg
        break;
      }  
      default:
        cerr << "Error: invalid option" << endl;
        exit(1);
    }  // switch ..choice
  }  // while
  if (routeoptions.mode == routeMode::kNone) {
    cerr << "Error: no mode specified" << endl;
    exit(1);
  }  // if ..mode
}  // getMode()

class Spaceship{
  private:
    char type;
    uint32_t levels;
    uint32_t size;
    struct Path {
      uint32_t level;
      uint32_t row;
      uint32_t col;
      Path() : level(0), row(0), col(0) {}
      Path(uint32_t l, uint32_t r, uint32_t c) : level(l), row(r), col(c){}
    };
    struct Square {
      char symbol;
      char dir;
      Square() : symbol('.'), dir(' ') {}
    };
    Square square;
    Path start;
    Path hanger;
    Path current;
    bool success = false;
    vector< vector< vector<Square> > > station;
    deque<Path> searchContainer;
    deque<Path> finalPath;
  public:
    Spaceship() : type('\0'), levels(0), size(0) {}

    void readfile(){
      cin >> type;
      if(type=='L'){
        readList();
      }
      else{
        readMap();
      }
    }

    void readMap(){
      string line;
      char ch;
      cin >> levels >> size;
      getline(cin, line);
      string comment;
      station.resize(levels, vector<vector<Square>>(size, vector<Square>(size, square)));
      for (uint32_t i = 0; i<levels;i++){
        for (uint32_t j = 0; j<size;j++){
          while(getline(cin, line)){
            if(line[0]=='/'){
              continue;
            }
            else{
              for (uint32_t k = 0; k<size;k++){
                ch = line[k];
                if(!(ch=='/'||ch=='.'||ch=='E'||ch=='#'||ch=='S'||ch=='H'||ch=='\n')){
                  cerr << "Error: Illegal map characters";
                  exit(1);
                }
                else{
                  if(ch=='S'){
                    start.level=i;
                    start.row=j;
                    start.col=k;
                  }  
                  square.symbol = ch;
                  station[i][j][k] = square;  
                }
              }
              break;
            }
          }
        }
      }
    }

    void readList(){
      uint32_t level, row, col;
      char ch, junk;
      string comment;
      cin >> levels >> size;
      station.resize(levels, vector<vector<Square>>(size, vector<Square>(size, square)));
      while(cin>>junk){
        if(junk=='/'){
          getline(cin, comment);
        }
        else{
          cin>>level>>junk>>row>>junk>>col>>junk>>ch>>junk;
          if(!(ch=='/'||ch=='.'||ch=='E'||ch=='#'||ch=='S'||ch=='H'||ch=='\n')){
            cerr << "Error: Illegal map characters";
            exit(1);
          }
          if(!(level<levels&&row<size&&col<size)){
            cerr << "Error: Input exceed bound";
            exit(1);
          }
          if(ch=='S'){
            start.level=level;
            start.row=row;
            start.col=col;
          }
          square.symbol=ch;
          station[level][row][col] = square;
        }
      }
    }
    void addFound(uint32_t level, uint32_t row, uint32_t col){
      Path found(level, row, col);
      searchContainer.push_back(found);
    }
    void addFinalPath(uint32_t level, uint32_t row, uint32_t col){
      Path found(level, row, col);
      finalPath.push_back(found);
    }

    bool findHanger(const Path current){
      bool pass = false;
      uint32_t l = current.level;
      uint32_t r = current.row;
      uint32_t c = current.col;
      if(r!=0&&station[l][r-1][c].symbol !='#'&&station[l][r-1][c].dir==' '){
        station[l][r-1][c].dir='n';
        if(station[l][r-1][c].symbol=='H'){
          success=true;
          hanger.level=l;
          hanger.row=r-1;
          hanger.col=c;
          return false;
        }
        addFound(l, r-1, c);
        pass = true;
      }
      if(c+1<size&&station[l][r][c+1].symbol!='#'&&station[l][r][c+1].dir==' '){
        station[l][r][c+1].dir='e';
        if(station[l][r][c+1].symbol=='H'){
          success=true;
          hanger.level=l;
          hanger.row=r;
          hanger.col=c+1;
          return false;
        }
        addFound(l, r, c+1);
        pass = true;
      }
      if(r+1<size&&station[l][r+1][c].symbol!='#'&&station[l][r+1][c].dir==' '){
        station[l][r+1][c].dir='s';
        if(station[l][r+1][c].symbol=='H'){
          success=true;
          hanger.level=l;
          hanger.row=r+1;
          hanger.col=c;
          return false;
        }
        addFound(l, r+1, c);
        pass = true;
      }
      if(c!=0&&station[l][r][c-1].symbol!='#'&&station[l][r][c-1].dir==' '){
        station[l][r][c-1].dir='w';
        if(station[l][r][c-1].symbol=='H'){
          success=true;
          hanger.level=l;
          hanger.row=r;
          hanger.col=c-1;
          return false;
        }
        addFound(l, r, c-1);
        pass = true;
      }
      if(station[l][r][c].symbol=='E'){
        for(uint32_t i = 0; i < levels;i++){
          Square &pos = station[i][r][c];
          if(i!=l&&pos.symbol=='E'&&pos.dir==' '){
            pos.dir=static_cast<char>('0' + l);
            addFound(i, r, c);
            pass = true;
          }
        }
      }
      return pass;
    }
    void stackSearch(){
      searchContainer.push_back(start);
      current = start;
      station[current.level][current.row][current.col].dir='o';
      while((!success)&&(findHanger(current)||searchContainer.size()!=0)){
        current = searchContainer.back();
        searchContainer.pop_back();
      }
    }
    void queueSearch(){
      searchContainer.push_back(start);
      current = start;
      station[current.level][current.row][current.col].dir='o';
      while((!success)&&(findHanger(current)||searchContainer.size()!=0)){
        current = searchContainer.front();
        searchContainer.pop_front();
      }
    }
    void traceBackList(){
      current  = hanger; 
      while(!(current.level==start.level&&current.row==start.row&&current.col==start.col)){
        char trace;
        trace = station[current.level][current.row][current.col].dir;
        switch(trace){
          case 'n':
            station[current.level][current.row+1][current.col].symbol = 'n';
            current.row = current.row+1;
            addFinalPath(current.level, current.row, current.col);
            break;
          case 'e':
            station[current.level][current.row][current.col-1].symbol = 'e';
            current.col = current.col-1;
            addFinalPath(current.level, current.row, current.col);
            break;
          case 's':
            station[current.level][current.row-1][current.col].symbol = 's';
            current.row = current.row-1;
            addFinalPath(current.level, current.row, current.col);
            break;
          case 'w':
            station[current.level][current.row][current.col+1].symbol = 'w';
            current.col = current.col+1;
            addFinalPath(current.level, current.row, current.col);
            break;
          default:
            uint32_t newlevel = static_cast<uint32_t>(trace - '0');
            station[newlevel][current.row][current.col].symbol = static_cast<char>('0' + current.level);
            current.level = newlevel;
            addFinalPath(current.level, current.row, current.col);
            break;
        }
      }
    }
    void traceBackMap(){
      current = hanger;
      while(!(current.level==start.level&&current.row==start.row&&current.col==start.col)){
        char trace;
        trace = station[current.level][current.row][current.col].dir;
        switch(trace){
          case 'n':
            station[current.level][current.row+1][current.col].symbol = 'n';
            current.row = current.row+1;
            break;
          case 'e':
            station[current.level][current.row][current.col-1].symbol = 'e';
            current.col = current.col-1;
            break;
          case 's':
            station[current.level][current.row-1][current.col].symbol = 's';
            current.row = current.row-1;
            break;
          case 'w':
            station[current.level][current.row][current.col+1].symbol = 'w';
            current.col = current.col+1;
            break;
          default:
            uint32_t newlevel = static_cast<uint32_t>(trace - '0');
            station[newlevel][current.row][current.col].symbol = static_cast<char>('0' + current.level);
            current.level = newlevel;
            break;
        }
      }
    }
    void printList(){
      cout << "//path taken\n";
      if (!success){
        return;
      }
      traceBackList();
      Path trace;
      while(!(finalPath.empty())){
        trace = finalPath.back();
        cout<<"("<<trace.level<<","<<trace.row<<","<<trace.col<<","<<station[trace.level][trace.row][trace.col].symbol<<")\n";
        finalPath.pop_back();
      }
    }
    void printMap(){
      if(success){
        traceBackMap();
      }
      cout << "Start in level "<<start.level<< ", row "<<start.row<<", column "<<start.col<<"\n";
      for (uint32_t i = 0; i<levels;i++){
        cout << "//level "<<i<<"\n";
        for (uint32_t j = 0; j<size;j++){
          for (uint32_t k = 0; k<size;k++){
            cout<<station[i][j][k].symbol;
          }
          cout << "\n";
        }
      }      
    }
};

int main(int argc, char *argv[]) {
  std::ios_base::sync_with_stdio(false);
  routeOptions routeoptions;
  outOptions outoptions;
  Spaceship boeing;
  getMode(argc, argv, routeoptions, outoptions);
  boeing.readfile();
  if (routeoptions.mode == routeMode::kQueue){
    boeing.queueSearch();
  }
  else if (routeoptions.mode == routeMode::kStack){
    boeing.stackSearch();
  }
  if (outoptions.mode == outMode::kMap){
    boeing.printMap();
  }
  else if(outoptions.mode == outMode::kList){
    boeing.printList();
  }
  return 0;
}