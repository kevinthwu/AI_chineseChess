/*************************************
* COMP3270 Artificial Intelligence
* Mini Project - Chinese Chess
*
* Author: Wu Tien Hsuan 2013500516
*
*************************************/
#include <iostream>
#include <cstdlib>
#include <algorithm>


using namespace std;

int moves=0; //number of moves of user
int LOOKAHEAD; //number of steps to look ahead
string openo[5]; //first five moves of user
string opend[5]; //first five moves of user

class Piece{
	public:
		char symbol; //the symbol to print out
		bool player; //0:computer 1:user
		int value; //value of the piece
		int locx; // x location
		int locy; // y location
		bool alive; //1:alive
		void pos(char s, bool p, int v, int x, int y); //set the status
};


void Piece::pos(char s, bool p, int v, int x, int y){ //set the configuration of the piece
	symbol=s;
	player=p;
	value=v;
	locx=x;
	locy=y;
	alive=1;
}

ostream& operator<<(ostream& cout, const Piece& p){
    cout << p.symbol;
    return cout;
}

class Board{
	public:
		int config[9][10];
		Piece pieces[33];
		Board* lc;
		Board* rs;
		int val; //total pieces value of computer-total pieces value of player, will be used to store backed-up value
		bool move(int ox, int oy, int dx, int dy, bool p); //return 1 if successful, p:player(0:computer 1:user)
		bool moveHint(int ox, int oy, int dx, int dy, bool p, bool h); //h: show error information
		Board* moveP(int ox, int oy, int dx, int dy, bool p); //without showing information
		bool validMove(Piece p, int dx, int dy); //(dx, dy) is the destination
		void init();
		void printBoard();
		void generateAll(int p); //generate all possible moves of player p
		void calVal(); //calculate value and update
		Board();
		
};

bool Board::validMove(Piece p, int dx, int dy){
	int ox=p.locx;
	int oy=p.locy;
	switch (p.symbol){
		case 'G':
			if((abs(dx-ox)==1&&abs(dy-oy)==0||abs(dy-oy)==1&&abs(dx-ox)==0)&&dx>=3&&dx<=5&&dy>=0&&dy<=2)
				return true;
			else if(pieces[config[dx][dy]].symbol=='g'&&ox==dx){ //flying general
				for(int i=min(oy, dy)+1; i<max(oy, dy); i++){
					if(config[dx][i]!=-1){
						return false;
					}
				}
				return true;
			}
			else
				return false;
			break;
		case 'g':
			if((abs(dx-ox)==1&&abs(dy-oy)==0||abs(dy-oy)==1&&abs(dx-ox)==0)&&dx>=3&&dx<=5&&dy>=7&&dy<=9)
				return true;
			else if(pieces[config[dx][dy]].symbol=='G'&&ox==dx){ //flying general
				for(int i=min(oy, dy)+1; i<max(oy, dy); i++){
					if(config[dx][i]!=-1){
						return false;
					}
				}
				return true;
			}
			else
				return false;
			break;
		case 'A':
			if((dx==4&&dy==1)&&(ox==3&&oy==0||ox==5&&oy==0||ox==3&&oy==2||ox==5&&oy==2)){
				return true;
			}else if((ox==4&&oy==1)&&(dx==3&&dy==0||dx==5&&dy==0||dx==3&&dy==2||dx==5&&dy==2)){
				return true;
			}else{
				return false;
			}
			break;
		case 'a':
			if((dx==4&&dy==8)&&(ox==3&&oy==9||ox==5&&oy==9||ox==3&&oy==7||ox==5&&oy==7)){
				return true;
			}else if((ox==4&&oy==8)&&(dx==3&&dy==9||dx==5&&dy==9||dx==3&&dy==7||dx==5&&dy==7)){
				return true;
			}else{
				return false;
			}
			break;
		
		case 'E':
			if(dy>4){
				return false;
			}
			if(abs(dx-ox)==2&&abs(dy-oy)==2){ //blocking the elephant's eye
				if(config[(dx+ox)/2][(dy+oy)/2]==-1){
					return true;
				}
			}
			return false;
			break;
			
		case 'e':
			if(dy<5){
				return false;
			}
			if(abs(dx-ox)==2&&abs(dy-oy)==2){ //blocking the elephant's eye
				if(config[(dx+ox)/2][(dy+oy)/2]==-1){
					return true;
				}
			}
			return false;
			break;
			
		case 'R':
		case 'r':
			if(dx==ox){
				for(int i=min(oy, dy)+1; i<max(oy, dy); i++){
					if(config[dx][i]!=-1){
						return false;
					}
				}
				return true;
			}else if(dy==oy){
				for(int i=min(ox, dx)+1; i<max(ox, dx); i++){
					if(config[i][dy]!=-1){
						return false;
					}
				}
				return true;
			}
			return false;
			break;
			
		case 'H':
		case 'h':
			if(abs(dx-ox)==1 && abs(dy-oy)==2){
				if(config[ox][(oy+dy)/2]==-1){ //check if not hobbling the horse's leg
					return true;
				}
			}
			if(abs(dx-ox)==2 && abs(dy-oy)==1){
				if(config[(ox+dx)/2][oy]==-1){ //check if not hobbling the horse's leg
					return true;
				}
			}
			return false;
			break;
			
		case 'C':
		case 'c':
			if(config[dx][dy]==-1){ //move to an empty space
				if(dx==ox){
					for(int i=min(oy, dy)+1; i<max(oy, dy); i++){
						if(config[dx][i]!=-1){
							return false;
						}
					}
					return true;
				}else if(dy==oy){
					for(int i=min(ox, dx)+1; i<max(ox, dx); i++){
						if(config[i][dy]!=-1){
							return false;
						}
					}
					return true;
				}
				return false;
			}else{// to capture
				int count=0;
				if(dx==ox){ 
					for(int i=min(oy, dy)+1; i<max(oy, dy); i++){
						if(config[dx][i]!=-1){
							count++;
						}
					}
				}else if(dy==oy){
					for(int i=min(ox, dx)+1; i<max(ox, dx); i++){
						if(config[i][dy]!=-1){
							count++;
						}
					}
				}
				if(count==1){
					return true;
				}else{
					return false;
				}
			}
			return false;
			break;
			
		case 'S':
			if(ox==dx&&dy-oy==1){
				return true;
			}else if(oy>4&&oy==dy&&abs(ox-dx)==1){ //crossed the river
				return true;
			}
			return false;
			break;
			
		case 's':
			if(ox==dx&&dy-oy==-1){
				return true;
			}else if(oy<5&&oy==dy&&abs(ox-dx)==1){ //crossed the river
				return true;
			}
			return false;
			break;	
	}
}

Board::Board(){
	lc=NULL;
	rs=NULL;
}

void Board::init(){
	
	for(int i=0; i<=8; i++){
		for(int j=0; j<=9; j++){
			config[i][j]=-1;
		}
	}
	
	pieces[0].pos('g', 0, 200, 4, 9); config[4][9]=0;
	pieces[1].pos('a', 0, 4, 3, 9); config[3][9]=1;
	pieces[2].pos('a', 0, 4, 5, 9); config[5][9]=2;
	pieces[3].pos('e', 0, 4, 2, 9); config[2][9]=3;
	pieces[4].pos('e', 0, 4, 6, 9); config[6][9]=4;
	pieces[5].pos('r', 0, 18, 0, 9); config[0][9]=5;
	pieces[6].pos('r', 0, 18, 8, 9); config[8][9]=6;
	pieces[7].pos('h', 0, 8, 1, 9); config[1][9]=7;
	pieces[8].pos('h', 0, 8, 7, 9); config[7][9]=8;
	pieces[9].pos('c', 0, 9, 1, 7); config[1][7]=9;
	pieces[10].pos('c', 0, 9, 7, 7); config[7][7]=10;
	pieces[11].pos('s', 0, 1, 0, 6); config[0][6]=11;
	pieces[12].pos('s', 0, 1, 2, 6); config[2][6]=12;
	pieces[13].pos('s', 0, 1, 4, 6); config[4][6]=13;
	pieces[14].pos('s', 0, 1, 6, 6); config[6][6]=14;
	pieces[15].pos('s', 0, 1, 8, 6); config[8][6]=15;
	
	pieces[16].pos('G', 1, 200, 4, 0); config[4][0]=16;
	pieces[17].pos('A', 1, 4, 3, 0); config[3][0]=17;
	pieces[18].pos('A', 1, 4, 5, 0); config[5][0]=18;
	pieces[19].pos('E', 1, 4, 2, 0); config[2][0]=19;
	pieces[20].pos('E', 1, 4, 6, 0); config[6][0]=20;
	pieces[21].pos('R', 1, 18, 0, 0); config[0][0]=21;
	pieces[22].pos('R', 1, 18, 8, 0); config[8][0]=22;
	pieces[23].pos('H', 1, 8, 1, 0); config[1][0]=23;
	pieces[24].pos('H', 1, 8, 7, 0); config[7][0]=24;
	pieces[25].pos('C', 1, 9, 1, 2); config[1][2]=25;
	pieces[26].pos('C', 1, 9, 7, 2); config[7][2]=26;
	pieces[27].pos('S', 1, 1, 0, 3); config[0][3]=27;
	pieces[28].pos('S', 1, 1, 2, 3); config[2][3]=28;
	pieces[29].pos('S', 1, 1, 4, 3); config[4][3]=29;
	pieces[30].pos('S', 1, 1, 6, 3); config[6][3]=30;
	pieces[31].pos('S', 1, 1, 8, 3); config[8][3]=31;

	lc==NULL;
	rs==NULL;
}

bool Board::move(int ox, int oy, int dx, int dy, bool p){
	return moveHint(ox, oy, dx, dy, p, 0);
}

bool Board::moveHint(int ox, int oy, int dx, int dy, bool p, bool h){
	if(ox<0||ox>8||dx<0||dx>8||oy<0||oy>9||dy<0||dy>9){
		if(h==1){
			cout << "Invalid move, out of boundary" << endl;
		}
		
		return 0;
	}
	else if(config[ox][oy]==-1){
		if(h==1){
			cout << "Invalid move, no piece available to move" << endl;
		}
		
		return 0;
	}else if(config[ox][oy]<=15 && p==1 || config[ox][oy]>=16 && p==0){
		if(h==1){
			cout << "Invalid move, not your piece" << endl;
		}
		return 0;
	}else if(config[dx][dy]!=-1 && config[dx][dy]<=15 && p==0 || config[dx][dy]!=-1 && config[dx][dy]>=16 && p==1){
		if(h==1){
				cout << "Invalid move, destination is your piece" << endl;
		}

		return 0;
	}else{
		if(validMove(pieces[config[ox][oy]], dx, dy)){
			if(config[dx][dy]!=-1){
				pieces[config[dx][dy]].locx=-1;
				pieces[config[dx][dy]].locy=-1;
				pieces[config[dx][dy]].alive=0;
			}
			pieces[config[ox][oy]].locx=dx;
			pieces[config[ox][oy]].locy=dy;
			config[dx][dy]=config[ox][oy];
			config[ox][oy]=-1;
			if(config[dx][dy]>=11&&config[dx][dy]<=15&&dy==4&&oy==5||config[dx][dy]>=27&&config[dx][dy]<=31&&dy==5&&oy==4){
				pieces[config[dx][dy]].value=4; //soldier crosses the river
			}
			return 1;
		}else{
			if(h==1){
				cout << "Invalid move" << endl;				
			}
			return 0;
		}
		
	}
	cout << endl;
}

Board* Board::moveP(int ox, int oy, int dx, int dy, bool p){
	//create new board
	Board* d = new Board();
	for(int i=0; i<=8; i++){
		for(int j=0; j<=9; j++){
			d->config[i][j]=config[i][j];
		}
	}
	for(int i=0; i<32; i++){
		d->pieces[i]=pieces[i];
	}
	d->lc=NULL;
	d->rs=NULL;
	
	if(d->move(ox, oy, dx, dy, p)==1){
		d->calVal();
		return d;
	}else{
		
		delete d;
		return NULL;
	}
}


void addChild(Board* o, Board* d){
	if(o->lc==NULL){
		o->lc = d;
	}else{ //add to head
		d->rs=o->lc->rs;
		o->lc->rs=d;
	}
}

void Board::generateAll(int p){ //generate all possible moves of player p
	Board* d;
	if(pieces[0].alive==0||pieces[16].alive==0){ 
		return;
	}
	if(p==0){
		if(pieces[5].alive){
			for(int i=0; i<9; i++){
				if((d=moveP(pieces[5].locx, pieces[5].locy, i, pieces[5].locy, 0))!=NULL){
					addChild(this, d);
				}
			}
			for(int i=0; i<10; i++){
				if((d=moveP(pieces[5].locx, pieces[5].locy, pieces[5].locx, i, 0))!=NULL){
					addChild(this, d);
				}
			}
		}
		
		if(pieces[6].alive){
			for(int i=0; i<9; i++){
				if((d=moveP(pieces[6].locx, pieces[6].locy, i, pieces[6].locy, 0))!=NULL){
					addChild(this, d);
				}
			}
			for(int i=0; i<10; i++){
				if((d=moveP(pieces[6].locx, pieces[6].locy, pieces[6].locx, i, 0))!=NULL){
					addChild(this, d);
				}
			}
		}
		
		if(pieces[7].alive){
			for(int i=pieces[7].locx-2; i<=pieces[7].locx+2; i++){
				for(int j=pieces[7].locy-2; j<=pieces[7].locy+2; j++){
					if((d=moveP(pieces[7].locx, pieces[7].locy, i, j, 0))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
		
		if(pieces[8].alive){
			for(int i=pieces[8].locx-2; i<=pieces[8].locx+2; i++){
				for(int j=pieces[8].locy-2; j<=pieces[8].locy+2; j++){
					if((d=moveP(pieces[8].locx, pieces[8].locy, i, j, 0))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
		
		if(pieces[11].alive){
			if((d=moveP(pieces[11].locx, pieces[11].locy, pieces[11].locx-1, pieces[11].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[11].locx, pieces[11].locy, pieces[11].locx+1, pieces[11].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[11].locx, pieces[11].locy, pieces[11].locx, pieces[11].locy-1, 0))!=NULL){
				addChild(this, d);
			}
		}
		if(pieces[12].alive){
			if((d=moveP(pieces[12].locx, pieces[12].locy, pieces[12].locx-1, pieces[12].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[12].locx, pieces[12].locy, pieces[12].locx+1, pieces[12].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[12].locx, pieces[12].locy, pieces[12].locx, pieces[12].locy-1, 0))!=NULL){
				addChild(this, d);
			}
		}
		if(pieces[13].alive){
			if((d=moveP(pieces[13].locx, pieces[13].locy, pieces[13].locx-1, pieces[13].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[13].locx, pieces[13].locy, pieces[13].locx+1, pieces[13].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[13].locx, pieces[13].locy, pieces[13].locx, pieces[13].locy-1, 0))!=NULL){
				addChild(this, d);
			}
		}
		if(pieces[14].alive){
			if((d=moveP(pieces[14].locx, pieces[14].locy, pieces[14].locx-1, pieces[14].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[14].locx, pieces[14].locy, pieces[14].locx+1, pieces[14].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[14].locx, pieces[14].locy, pieces[14].locx, pieces[14].locy-1, 0))!=NULL){
				addChild(this, d);
			}
		}
		if(pieces[15].alive){
			if((d=moveP(pieces[15].locx, pieces[15].locy, pieces[15].locx-1, pieces[15].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[15].locx, pieces[15].locy, pieces[15].locx+1, pieces[15].locy, 0))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[15].locx, pieces[15].locy, pieces[15].locx, pieces[15].locy-1, 0))!=NULL){
				addChild(this, d);
			}
		}
		
		if(pieces[9].alive){
			for(int i=0; i<9; i++){
				if((d=moveP(pieces[9].locx, pieces[9].locy, i, pieces[9].locy, 0))!=NULL){
					addChild(this, d);
				}
			}
			for(int i=0; i<10; i++){
				if((d=moveP(pieces[9].locx, pieces[9].locy, pieces[9].locx, i, 0))!=NULL){
					addChild(this, d);
				}
			}
		}
		
		if(pieces[10].alive){
			for(int i=0; i<9; i++){
				if((d=moveP(pieces[10].locx, pieces[10].locy, i, pieces[10].locy, 0))!=NULL){
					addChild(this, d);
				}
			}
			for(int i=0; i<10; i++){
				if((d=moveP(pieces[10].locx, pieces[10].locy, pieces[10].locx, i, 0))!=NULL){
					addChild(this, d);
				}
			}
		}
		if(pieces[1].alive){
			if((d=moveP(pieces[1].locx, pieces[1].locy, 3, 9, 0))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[1].locx, pieces[1].locy, 5, 9, 0))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[1].locx, pieces[1].locy, 4, 8, 0))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[1].locx, pieces[1].locy, 3, 7, 0))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[1].locx, pieces[1].locy, 5, 7, 0))!=NULL){
					addChild(this, d);
			}
		}
		if(pieces[2].alive){
			if((d=moveP(pieces[2].locx, pieces[2].locy, 3, 9, 0))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[2].locx, pieces[2].locy, 5, 9, 0))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[2].locx, pieces[2].locy, 4, 8, 0))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[2].locx, pieces[2].locy, 3, 7, 0))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[2].locx, pieces[2].locy, 5, 7, 0))!=NULL){
					addChild(this, d);
			}
		}
		if(pieces[3].alive){
			for(int i=0; i<9; i+=2){
				for(int j=5; j<10; j+=2){
					if((d=moveP(pieces[3].locx, pieces[3].locy, i, j, 0))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
		if(pieces[4].alive){
			for(int i=0; i<9; i+=2){
				for(int j=5; j<10; j+=2){
					if((d=moveP(pieces[4].locx, pieces[4].locy, i, j, 0))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
		if(pieces[0].alive){
			for(int i=3; i<=5; i++){
				for(int j=7; j<=9; j++){
					if((d=moveP(pieces[0].locx, pieces[0].locy, i, j, 0))!=NULL){
						addChild(this, d);
					}
				}
			}
			for(int i=3; i<=5; i++){ //flying general
				for(int j=0; j<=2; j++){
					if((d=moveP(pieces[0].locx, pieces[0].locy, i, j, 0))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
	}
	if(p==1){
		if(pieces[21].alive){
			for(int i=0; i<9; i++){
				if((d=moveP(pieces[21].locx, pieces[21].locy, i, pieces[21].locy, 1))!=NULL){
					addChild(this, d);
				}
			}
			for(int i=0; i<10; i++){
				if((d=moveP(pieces[21].locx, pieces[21].locy, pieces[21].locx, i, 1))!=NULL){
					addChild(this, d);
				}
			}
		}
		
		if(pieces[22].alive){
			for(int i=0; i<9; i++){
				if((d=moveP(pieces[22].locx, pieces[22].locy, i, pieces[22].locy, 1))!=NULL){
					addChild(this, d);
				}
			}
			for(int i=0; i<10; i++){
				if((d=moveP(pieces[22].locx, pieces[22].locy, pieces[22].locx, i, 1))!=NULL){
					addChild(this, d);
				}
			}
		}
		
		if(pieces[23].alive){
			for(int i=pieces[23].locx-2; i<=pieces[23].locx+2; i++){
				for(int j=pieces[23].locy-2; j<=pieces[23].locy+2; j++){
					if((d=moveP(pieces[23].locx, pieces[23].locy, i, j, 1))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
		
		if(pieces[24].alive){
			for(int i=pieces[24].locx-2; i<=pieces[24].locx+2; i++){
				for(int j=pieces[24].locy-2; j<=pieces[24].locy+2; j++){
					if((d=moveP(pieces[24].locx, pieces[24].locy, i, j, 1))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
		
		if(pieces[27].alive){
			if((d=moveP(pieces[27].locx, pieces[27].locy, pieces[27].locx-1, pieces[27].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[27].locx, pieces[27].locy, pieces[27].locx+1, pieces[27].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[27].locx, pieces[27].locy, pieces[27].locx, pieces[27].locy+1, 1))!=NULL){
				addChild(this, d);
			}
		}
		if(pieces[28].alive){
			if((d=moveP(pieces[28].locx, pieces[28].locy, pieces[28].locx-1, pieces[28].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[28].locx, pieces[28].locy, pieces[28].locx+1, pieces[28].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[28].locx, pieces[28].locy, pieces[28].locx, pieces[28].locy+1, 1))!=NULL){
				addChild(this, d);
			}
		}
		if(pieces[29].alive){
			if((d=moveP(pieces[29].locx, pieces[29].locy, pieces[29].locx-1, pieces[29].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[29].locx, pieces[29].locy, pieces[29].locx+1, pieces[29].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[29].locx, pieces[29].locy, pieces[29].locx, pieces[29].locy+1, 1))!=NULL){
				addChild(this, d);
			}
		}
		if(pieces[30].alive){
			if((d=moveP(pieces[30].locx, pieces[30].locy, pieces[30].locx-1, pieces[30].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[30].locx, pieces[30].locy, pieces[30].locx+1, pieces[30].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[30].locx, pieces[30].locy, pieces[30].locx, pieces[30].locy+1, 1))!=NULL){
				addChild(this, d);
			}
		}
		if(pieces[31].alive){
			if((d=moveP(pieces[31].locx, pieces[31].locy, pieces[31].locx-1, pieces[31].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[31].locx, pieces[31].locy, pieces[31].locx+1, pieces[31].locy, 1))!=NULL){
				addChild(this, d);
			}
			if((d=moveP(pieces[31].locx, pieces[31].locy, pieces[31].locx, pieces[31].locy+1, 1))!=NULL){
				addChild(this, d);
			}
		}
		
		if(pieces[25].alive){
			for(int i=0; i<9; i++){
				if((d=moveP(pieces[25].locx, pieces[25].locy, i, pieces[25].locy, 1))!=NULL){
					addChild(this, d);
				}
			}
			for(int i=0; i<10; i++){
				if((d=moveP(pieces[25].locx, pieces[25].locy, pieces[25].locx, i, 1))!=NULL){
					addChild(this, d);
				}
			}
		}
		
		if(pieces[26].alive){
			for(int i=0; i<9; i++){
				if((d=moveP(pieces[26].locx, pieces[26].locy, i, pieces[26].locy, 1))!=NULL){
					addChild(this, d);
				}
			}
			for(int i=0; i<10; i++){
				if((d=moveP(pieces[26].locx, pieces[26].locy, pieces[26].locx, i, 1))!=NULL){
					addChild(this, d);
				}
			}
		}
		if(pieces[17].alive){
			if((d=moveP(pieces[17].locx, pieces[17].locy, 3, 0, 1))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[17].locx, pieces[17].locy, 5, 0, 1))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[17].locx, pieces[17].locy, 4, 1, 1))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[17].locx, pieces[17].locy, 3, 2, 1))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[17].locx, pieces[17].locy, 5, 2, 1))!=NULL){
					addChild(this, d);
			}
		}
		if(pieces[18].alive){
			if((d=moveP(pieces[18].locx, pieces[18].locy, 3, 0, 1))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[18].locx, pieces[18].locy, 5, 0, 1))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[18].locx, pieces[18].locy, 4, 1, 1))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[18].locx, pieces[18].locy, 3, 2, 1))!=NULL){
					addChild(this, d);
			}
			if((d=moveP(pieces[18].locx, pieces[18].locy, 5, 2, 1))!=NULL){
					addChild(this, d);
			}
		}
		if(pieces[19].alive){
			for(int i=0; i<9; i+=2){
				for(int j=0; j<5; j+=2){
					if((d=moveP(pieces[19].locx, pieces[19].locy, i, j, 1))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
		if(pieces[20].alive){
			for(int i=0; i<9; i+=2){
				for(int j=0; j<5; j+=2){
					if((d=moveP(pieces[20].locx, pieces[20].locy, i, j, 1))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
		if(pieces[16].alive){
			for(int i=3; i<=5; i++){
				for(int j=0; j<=2; j++){
					if((d=moveP(pieces[16].locx, pieces[16].locy, i, j, 1))!=NULL){
						addChild(this, d);
					}
				}
			}
			for(int i=3; i<=5; i++){ //flying general
				for(int j=7; j<=9; j++){
					if((d=moveP(pieces[16].locx, pieces[16].locy, i, j, 1))!=NULL){
						addChild(this, d);
					}
				}
			}
		}
	}
}

void Board::calVal(){
	val=0;
	for(int i=0; i<16; i++){ //computer
		if(pieces[i].alive){
			val+=pieces[i].value;
		}
	}
	for(int i=16; i<32; i++){ //user
		if(pieces[i].alive){
			val-=pieces[i].value;
		}
	}
}

void Board::printBoard(){ //print out the chess board
	int j=9;
	cout << endl << "   0 1 2 3 4 5 6 7 8 " << endl << endl;
	for(;j>=5; j--){
		cout << (char)('a'+j) << "  ";
		for(int i=0; i<=8; i++){
			if(config[i][j]==-1){
				cout << ". ";
			}else{
				cout << pieces[config[i][j]] << " ";
			}
		}
		
		cout << " " << (char)('a'+j) << endl;
	}

	cout << endl;
	
	for(;j>=0; j--){
		cout << (char)('a'+j) << "  ";
		for(int i=0; i<=8; i++){
			if(config[i][j]==-1){
				cout << ". ";
			}else{
				cout << pieces[config[i][j]] << " ";
			}
		}
		cout << " " << (char)('a'+j) << endl;
	}
	cout <<endl <<  "   0 1 2 3 4 5 6 7 8 " << endl << endl;
	
}

void userMove(Board *b){ //prompt user to enter the move
	string s1, s2;                          
	cout << "Your move: ";
	cin >> s1 >> s2;
	while(b->moveHint(s1[1]-'0', s1[0]-'a', s2[1]-'0', s2[0]-'a', 1, 1) == 0){  
		cin.clear();
		fflush(stdin);                 
		cout << "Your move: ";
		cin >> s1 >> s2;
	} //do not terminate until success
	if(moves<5){
			openo[moves]=s1;
			opend[moves]=s2;
	}
	moves++;
}


int minValue(Board* b, int x, int y, int term);

int maxValue(Board* b, int x, int y, int term){	
	int v=-800;
	term--;
	if(term==0){
		v=b->val;
		return v;
	}
	b->generateAll(0);
	if(b->lc==NULL){
		return b->val;
	}
	Board* z=b->lc;
	while(z!=NULL){
		v=max(v, minValue(z, x, y, term));
		b->val=v;
		if(v>=y){
			break;
		}
		x=max(x,v);
		z=z->rs;
		
	}
	z=b->lc;
	if (term < LOOKAHEAD-1){
		while(z!=NULL){
			Board* toDel=z;
			z=z->rs;
			delete toDel;
		}		
	}
	
	return v;
}

int minValue(Board* b, int x, int y, int term){
	int v=800;
	term--;
	if(term==0){
		v=b->val;
		return v;
	}
	b->generateAll(0);
	if(b->lc==NULL){
		return b->val;
	}
	b->generateAll(1);
	Board* z=b->lc;
	while(z!=NULL){
		Board* toDel=z;
		v=min(v, maxValue(z, x, y, term));
		b->val=v;
		if(v<=x){
			break;
		}
		y=min(y,v);

		z=z->rs;
	}
	z=b->lc;
	if (term < LOOKAHEAD-1){
		while(z!=NULL){
			Board* toDel=z;
			z=z->rs;
			delete toDel;
		}		
	}
	return v;
}

Board* alphaBetaSearch(Board* b){
	b->val=maxValue(b, -800, 800, LOOKAHEAD);
	Board* x=b->lc;
	if(b!=NULL){
		while(x!=NULL){
			if(x->val==b->val){
				return x;
			}else{
				x=x->rs;
			}
		}
	}
	cout << "ERROR" << endl;
	return NULL;
}

Board* databaseMove(Board *b){
	if(opend[0]=="c4"){		
		if(openo[0]=="c1"||openo[0]=="c7"){ //ECCO=D0
			if(moves==1)
			return b->moveP(1,7,4,7,0);
		}
	}
	if(opend[0]=="c6"){		 //ECCO=A06
		if(openo[0]=="c7"){
			if(moves==1)
			return b->moveP(7,9,8,7,0);
		}
	}
	if(opend[0]=="c2"){		 //ECCO=A06
		if(openo[0]=="c1"){
			if(moves==1)
			return b->moveP(1,9,0,7,0);
		}
	}
	if(opend[0]=="c5"){		 //ECCO=A51
		if(openo[0]=="c7"){
			if(moves==1)
			return b->moveP(7,9,6,7,0);
		}
	}
	if(opend[0]=="c3"){		 //ECCO=A51
		if(openo[0]=="c1"){
			if(moves==1)
			return b->moveP(1,9,2,7,0);
		}
	}
	if(opend[0]=="c3"){		 //ECCO=A62
		if(openo[0]=="c7"){
			if(moves==1)
			return b->moveP(8,9,8,8,0);
		}
	}
	if(opend[0]=="c5"){		 //ECCO=A62
		if(openo[0]=="c1"){
			if(moves==1)
			return b->moveP(0,9,0,8,0);
		}
	}
	if(opend[0]=="c2"){		 //ECCO=A07
		if(openo[0]=="c7"){
			if(moves==1)
			return b->moveP(2,6,2,5,0);
		}
	}
	if(opend[0]=="c6"){		 //ECCO=A07
		if(openo[0]=="c1"){
			if(moves==1)
			return b->moveP(6,6,6,5,0);
		}
	}
	if(opend[0]=="e7"){		 //ECCO=A05
		if(openo[0]=="c7"){
			if(moves==1)
			return b->moveP(7,9,8,8,0);
		}
	}
	if(opend[0]=="e1"){		 //ECCO=A05
		if(openo[0]=="c1"){
			if(moves==1)
			return b->moveP(1,9,2,7,0);
		}
	}
	if(opend[0]=="e2"){		 //ECCO=E1
		if(openo[0]=="d2"){
			if(moves==1){
				return b->moveP(1,7,2,7,0);	
			}
			if(opend[1]=="c4"){
				if(openo[1]=="c7"){
					if(moves==2){
						return b->moveP(2,9,4,7,0);
					}
					if(opend[2]=="c6"){
						if(openo[2]=="a7"){
							if(moves==3){
								return b->moveP(2,6,2,5,0);
							}
							if(opend[3]=="a7"){
								if(openo[3]=="a8"){
									if(moves==4){
										return b->moveP(2,5,2,4,0);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	if(opend[0]=="e6"){		 //ECCO=E1
		if(openo[0]=="d6"){
			if(moves==1){
				return b->moveP(7,7,6,7,0);	
			}
			if(opend[1]=="c4"){
				if(openo[1]=="c1"){
					if(moves==2){
						return b->moveP(6,9,4,7,0);
					}
					if(opend[2]=="c2"){
						if(openo[2]=="a1"){
							if(moves==3){
								return b->moveP(6,6,6,5,0);
								if(opend[3]=="a1"){
									if(openo[3]=="a0"){
										if(moves==4){
											return b->moveP(6,5,6,4,0);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	if(opend[0]=="e8"){		 //ECCO=A08
		if(openo[0]=="d8"){
			if(moves==1)
			return b->moveP(2,6,2,5,0);
		}
	}
	if(opend[0]=="e0"){		 //ECCO=A08
		if(openo[0]=="d0"){
			if(moves==1)
			return b->moveP(6,6,6,5,0);
		}
	}
	
	if(opend[0]=="c6"){		 //ECCO=A40
		if(openo[0]=="a7"){
			if(moves==1)
			return b->moveP(1,9,2,7,0);
		}
	}
	if(opend[0]=="c2"){		 //ECCO=A40
		if(openo[0]=="a1"){
			if(moves==1)
			return b->moveP(7,9,2,7,0);
		}
	}
	
	if(opend[0]=="c8"){		 //ECCO=A02
		if(openo[0]=="a7"){
			if(moves==1)
			return b->moveP(1,7,4,7,0);
		}
	}
	if(opend[0]=="c0"){		 //ECCO=A02
		if(openo[0]=="a1"){
			if(moves==1)
			return b->moveP(7,7,4,7,0);
		}
	}
	
	if(opend[0]=="b4"){		 //ECCO=A01
		if(openo[0]=="a3"){
			if(moves==1)
			return b->moveP(1,9,2,7,0);
		}
	}
	if(opend[0]=="b4"){		 //ECCO=A01
		if(openo[0]=="a5"){
			if(moves==1)
			return b->moveP(7,9,6,7,0);
		}
	}
	
	
	return NULL;
}

void printComputerMove(Board *b, Board *result){
	
		//print out the move by the computer
	for(int i=0; i<9; i++){
		for(int j=0; j<10; j++){
			if(b->config[i][j]<16&&b->config[i][j]>=0&&result->config[i][j]==-1){
				cout << "Computer moves from " << (char) ('a'+j) << i << " to ";
			}
		}
	}
	
	for(int i=0; i<9; i++){
		for(int j=0; j<10; j++){
			if(result->config[i][j]<16&&result->config[i][j]>=0&&(b->config[i][j]==-1||b->config[i][j]>=16)){
				cout << (char) ('a'+j) << i << " ... (" << result->pieces[result->config[i][j]] << ")" << endl;
			}
			
			if(result->config[i][j]<16&&result->config[i][j]>=0&&b->config[i][j]>=16){
				cout << "Your (" << b->pieces[b->config[i][j]] << ") is captured" << endl;
			}
			
		}
	}
}

Board* computerMove(Board *b){

	Board* result=NULL;
	
	if(moves<5){
		result = databaseMove(b);
		if(result!=NULL){
			printComputerMove(b, result);
			return result;
		}
	}


	Board* x; //traverse the siblings

	result = alphaBetaSearch(b);
	
	printComputerMove(b, result);

	x=b->lc;
	while(x!=NULL){
		if(x!=result){
			Board* z=x;
			x=x->rs;
			delete(z);
		}else if(x==result){
			x=x->rs;
		}
	}
	result->lc=NULL;
	result->rs=NULL;
	return result;

	
}

int main(){
	cout << "Please enter the number of steps (5 or 6) to look ahead. The waiting time within 5 steps is short, and the waiting time for 6 steps varies from 10 seconds to 30 seconds." << endl;
	cin >> LOOKAHEAD;
	while(LOOKAHEAD<5 or LOOKAHEAD>6){
		cout << "Please enter again: ";
		cin >> LOOKAHEAD;
	}
	cout << "Please enter the origin and the destination of your move (separated by a space) e.g. \" c1 c4 \"" << endl ;
	cout << "The program will look ahead of " << LOOKAHEAD << " steps." << endl << endl;
	Board* b = new Board();
	b->init();
	b->printBoard();
	cout << endl;
	
	while(true){
		Board* temp;
		userMove(b);
		if(b->pieces[0].alive==0){
			b->printBoard(); 
			cout << "You win!!" << endl;
			break;
		}
		b->printBoard();
		cout << "Searching..." << endl;
		temp=b;
		b=computerMove(b);
		delete temp;  

		if(b->pieces[16].alive==0){
			b->printBoard(); 
			cout << "You lose..." << endl;
			break;
		}
		cout << "Number of user moves: " << moves << endl;
		b->printBoard();
	}
	delete b;
	getchar();
	return 0;
}

