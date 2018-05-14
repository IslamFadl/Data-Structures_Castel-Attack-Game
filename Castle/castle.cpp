
#include <iostream>
#include <fstream> // file stream
#include <string>
using namespace std;
#include "utility.h"
#include "binhp.h"


int main()
{

	ifstream fin; // object input file stream
	fin.open("data.txt");
	// check for errors
	if(fin.fail())
	{
		cerr << "Error opening the file" << endl; // c error for cout-ing errors
		exit(1); // to exit the program without crashing
	}
	//\\            \\//
	//\\ ********** \\//
	//\\            \\//
	int TH, NE, TP , C1 , C2 , C3 , numOfEnemy = 8;
	fin >> TH;
	fin >> NE;
	fin >> TP;	// all towers fire power
	fin >> C1;
	fin >> C2;
	fin >> C3;

	enemy* enemies = new enemy[numOfEnemy];
	for (int i=0; i<numOfEnemy ; ++i)
	{
		fin >> enemies[i].ID;
		fin >> enemies[i].Arrival_Time;
		fin >> enemies[i].Health;
		fin >> enemies[i].Power;
		fin >> enemies[i].Hit_Period;
		fin >> enemies[i].Type;
		fin >> enemies[i].Region;
		enemies[i].timeRemainToShoot = enemies[i].Hit_Period;
		enemies[i].k = 1;
		enemies[i].chkd = false;
		if (enemies[i].Type == 3) enemies[i].k = 2;		// shielded enemy
		enemies[i].Distance = 60;						// out of the screen
		enemies[i].killchk = 0;
		 
		//cout << enemies[i].ID << "\t" << enemies[i].Arrival_Time << "\t" << enemies[i].Health << "\t" << enemies[i].Power << "\t" << enemies[i].Hit_Period << "\t" << enemies[i].Type << "\t" << enemies[i].Region << endl;
	}

	fin.close();
	ofstream fout;
	fout.open("stats.txt");

	SetWindow();
	castle ct; // define castle 

	ct.Xstrt = CastleXStrt;
	ct.Ystrt = CastleYStrt;
	ct.W = CastleWidth;
	ct.L = CastleLength;
	//intialize health of the towers
	ct.towers[0].Health = TH; int chk1 = 0;
	ct.towers[1].Health = TH; int chk2 = 0;
	ct.towers[2].Health = TH; int chk3 = 0;
	ct.towers[3].Health = TH; int chk4 = 0;

	///You should draw the castle first then draw the enemies
	DrawCastle(ct);//,12);
	DrawEnemies(enemies, numOfEnemy);
	/////////////////////////////////
	//use this function to print your messages
	//PrintMsg("Total current eneimes  A = 0\tB = 0\tC = 0\tD = 0 \n");
	//PrintMsg("Last time step kill    A = 0\tB = 0\tC = 0\tD = 0 \n");
	//PrintMsg("Total killed by        A = 0\tB = 0\tC = 0\tD = 0 \n");
	//PrintMsg("Un paved distance      A = 0\tB = 0\tC = 0\tD = 0 \n");

	//FD hitperiod
	//KD t_killed - (FD+arrival)
	//FT FD+KD

	binheap hp1(numOfEnemy), hp2(numOfEnemy), hp3(numOfEnemy), hp4(numOfEnemy);
	int t = 0, allowedMove = 10, towersKilled=0, killedEnemies=0;
	float DamagEnemyTower, DamagTowerEnemy, minDisToCastle1=30, minDisToCastle2=30, minDisToCastle3=30, minDisToCastle4=30;
	/////////////////////////////
	/////////////////////////////
	/////////////////////////////
	while(1)
	{
			///////////////////////////////
			/// Start of first for loop ///
			///////////////////////////////
			for (int ii=0; ii<numOfEnemy ; ++ii)
			{
				if (enemies[ii].Arrival_Time <= t)	// lsa wasel aw wsl mn abl kda
				{
					DamagEnemyTower = enemies[ii].Power/enemies[ii].Distance;	// damage of paver is distance it can pave forward
					enemies[ii].Priority = (DamagEnemyTower*C1/(enemies[ii].timeRemainToShoot+1))+(enemies[ii].Health*C2)+(enemies[ii].Type*C3);
					enemies[ii].timeRemainToShoot -= 1;
					if (enemies[ii].timeRemainToShoot == 0) enemies[ii].timeRemainToShoot = enemies[ii].Hit_Period;
					//////////////////////////
					//////////////////////////
					//////////////////////////
					if (enemies[ii].Region == 1)
					{
						if ((enemies[ii].Health>0) && enemies[ii].chkd == false)
						{ hp1.insert(enemies[ii]); enemies[ii].chkd = true; }
							if (enemies[ii].Type != 1)
							{
								ct.towers[0].Health -= DamagEnemyTower;
								if (ct.towers[0].Health <= 0 && chk1==0 )
								{	towersKilled += 1;
								    chk1 = 1;
									if (towersKilled == 4)
									{
										fout << "==========================================" << endl;
										fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
										fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
										fout.close();
										return -2;
									}
									for (int iii=0; iii<numOfEnemy ; ++iii) // should loop over all enemies because they all may exist in onr region if
									{										// all other region's towers are destroyed
										if (enemies[iii].Region == 1) enemies[iii].Region = 2;
									}
								}
							}
							else
							{if (enemies[ii].Distance <= minDisToCastle1 && minDisToCastle1 > 0) 
								minDisToCastle1 -= DamagEnemyTower;}
						if (enemies[ii].Distance > minDisToCastle1 && enemies[ii].Distance > 0)	enemies[ii].Distance -= allowedMove;
					}
					//////////////////////////
					//////////////////////////
					//////////////////////////
					if (enemies[ii].Region == 2)
					{
						if ((enemies[ii].Health>0) && enemies[ii].chkd == false)
						{ hp2.insert(enemies[ii]); enemies[ii].chkd = true; }
							if (enemies[ii].Type != 1)
							{
								ct.towers[1].Health -= DamagEnemyTower;
								if (ct.towers[1].Health <= 0 && chk2==0) 
								{
									chk2 = 1;
									towersKilled += 1;
									if (towersKilled == 4)
									{
										fout << "==========================================" << endl;
										fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
										fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
										fout.close();
										return -2;
									}
									for (int iii=0; iii<numOfEnemy ; ++iii)
									{
										if(enemies[iii].Region == 2) enemies[iii].Region = 3;
									}
								}
							}
							else
							{if (enemies[ii].Distance <= minDisToCastle2 && minDisToCastle2 > 0) 
								minDisToCastle2 -= DamagEnemyTower;}
						if (enemies[ii].Distance > minDisToCastle2 && enemies[ii].Distance > 0)	enemies[ii].Distance -= allowedMove;
					}
					//////////////////////////
					//////////////////////////
					//////////////////////////
					if (enemies[ii].Region == 3)
					{
						if ((enemies[ii].Health>0) && enemies[ii].chkd == false)
						{ hp3.insert(enemies[ii]); enemies[ii].chkd = true; }
							if (enemies[ii].Type != 1)
							{
								ct.towers[2].Health -= DamagEnemyTower;
								if (ct.towers[2].Health <= 0 && chk3==0) 
								{
									chk3 = 1;
									towersKilled += 1;
									if (towersKilled == 4)
									{
										fout << "==========================================" << endl;
										fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
										fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
										fout.close();
										return -2;
									}
									for (int iii=0; iii<numOfEnemy ; ++iii)
									{
										if (enemies[iii].Region == 3) enemies[iii].Region = 4;
									}
								}
							}
							else
							{if (enemies[ii].Distance <= minDisToCastle3 && minDisToCastle3 > 0) 
								minDisToCastle3 -= DamagEnemyTower;}
						if (enemies[ii].Distance > minDisToCastle3 && enemies[ii].Distance > 0)	enemies[ii].Distance -= allowedMove;
					}
					//////////////////////////
					//////////////////////////
					//////////////////////////
					if (enemies[ii].Region == 4)
					{
						if ((enemies[ii].Health>0) && enemies[ii].chkd == false)
						{ hp4.insert(enemies[ii]); enemies[ii].chkd = true; }
							if (enemies[ii].Type != 1)
							{
								ct.towers[3].Health -= DamagEnemyTower;
								if (ct.towers[3].Health <= 0 && chk4==0) 
								{
									chk4 = 1;
									towersKilled += 1;
									if (towersKilled == 4)
									{
										fout << "==========================================" << endl;
										fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
										fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
										return -2;
									}
									for (int iii=0; iii<numOfEnemy ; ++iii)
									{
										if (enemies[iii].Region == 4) enemies[iii].Region = 1;
									}
								}
							}
							else
							{if (enemies[ii].Distance <= minDisToCastle4 && minDisToCastle4 > 0) 
								minDisToCastle4 -= DamagEnemyTower;}
						if (enemies[ii].Distance > minDisToCastle4 && enemies[ii].Distance > 0)	enemies[ii].Distance -= allowedMove;

					}
					//////////////////////////
					//////////////////////////
					//////////////////////////
				}	// end of condition of arriving time
			}
			/////////////////////////////
			/// End of first for loop ///
			/////////////////////////////
			////////////////////////////////
			/// Start of second for loop ///
			////////////////////////////////
			for (int jj=0; jj<NE ; ++jj)		// number of enemies that tower can deal with at once "at one time step"
			{
				int ID1 = hp1.removeMax();
				enemies[ID1].chkd = false;
				int ID2 = hp2.removeMax();
				enemies[ID2].chkd = false;
				int ID3 = hp3.removeMax();
				enemies[ID3].chkd = false;
				int ID4 = hp4.removeMax();
				enemies[ID4].chkd = false;

				if (ID1 >= 0) // -1 then region 1 is empty but it might be filled in the future if enemies are transferred from region 4 to it
				{
					DamagTowerEnemy = TP / (enemies[ID1].Distance * enemies[ID1].k);
					if (enemies[ID1].Health > 0) enemies[ID1].Health -= DamagTowerEnemy;

					else
					{
						int KD = t-(enemies[ID1].Hit_Period+enemies[ID1].Arrival_Time);
						int FT = KD + enemies[ID1].Hit_Period;
						fout<<t<<"\t"<<enemies[ID1].ID<<"\t"<<enemies[ID1].Hit_Period<<"\t"<<KD<<"\t"<<FT<<endl;
						if (enemies[ID1].killchk == 0)
						{
							enemies[ID1].killchk = 1;
							enemies[ID1].Distance = 61;
							killedEnemies += 1;
						}
						if (killedEnemies == numOfEnemy)
						{
							fout << "==========================================" << endl;
							fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
							fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
							fout.close();
							return -2;
						}
					}
				}

				if (ID2 >= 0)
				{
					DamagTowerEnemy = TP / (enemies[ID2].Distance * enemies[ID2].k);
					if (enemies[ID2].Health > 0) enemies[ID2].Health -= DamagTowerEnemy;
					else
					{
						int KD = t-(enemies[ID2].Hit_Period+enemies[ID2].Arrival_Time);
						int FT = KD + enemies[ID2].Hit_Period;
						fout<<t<<"\t"<<enemies[ID2].ID<<"\t"<<enemies[ID2].Hit_Period<<"\t"<<KD<<"\t"<<FT<<endl;
						if (enemies[ID2].killchk == 0)
						{
							enemies[ID2].killchk = 1;
							enemies[ID2].Distance = 61;
							killedEnemies += 1;
						}
						if (killedEnemies == numOfEnemy)
						{
							fout << "==========================================" << endl;
							fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
							fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
							fout.close();
							return -2;
						}
					}
				}

				if (ID3 >= 0)
				{
					DamagTowerEnemy = TP / (enemies[ID3].Distance * enemies[ID3].k);
					if (enemies[ID3].Health > 0) enemies[ID3].Health -= DamagTowerEnemy;
					else
					{
						int KD = t-(enemies[ID3].Hit_Period+enemies[ID3].Arrival_Time);
						int FT = KD + enemies[ID3].Hit_Period;
						fout<<t<<"\t"<<enemies[ID3].ID<<"\t"<<enemies[ID3].Hit_Period<<"\t"<<KD<<"\t"<<FT<<endl;
						if (enemies[ID3].killchk == 0)
						{
							enemies[ID3].killchk = 1;
							enemies[ID3].Distance = 61;
							killedEnemies += 1;
						}
						if (killedEnemies == numOfEnemy)
						{
							fout << "==========================================" << endl;
							fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
							fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
							fout.close();
							return -2;
						}
					}
				}

				if (ID4 >= 0)
				{
					DamagTowerEnemy = TP / (enemies[ID4].Distance * enemies[ID4].k);
					if (enemies[ID4].Health > 0) enemies[ID4].Health -= DamagTowerEnemy;
					else
					{
						int KD = t-(enemies[ID4].Hit_Period+enemies[ID4].Arrival_Time);
						int FT = KD + enemies[ID4].Hit_Period;
						fout<<t<<"\t"<<enemies[ID4].ID<<"\t"<<enemies[ID4].Hit_Period<<"\t"<<KD<<"\t"<<FT<<endl;
						if (enemies[ID4].killchk == 0)
						{
							enemies[ID4].killchk = 1;
							enemies[ID4].Distance = 61;
							killedEnemies += 1;
						}
						if (killedEnemies == numOfEnemy)
						{
							fout << "==========================================" << endl;
							fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
							fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
							fout.close();
							return -2;
						}
					}
				}
				if (ID1 == -1 && ID2 == -1 && ID3 == -1 && ID4 == -1)
				{
					fout << "==========================================" << endl;
					fout <<200-ct.towers[0].Health<<"\t"<<200-ct.towers[1].Health<<"\t"<< 200-ct.towers[2].Health<<"\t"<<200-ct.towers[3].Health<<endl;
					fout <<minDisToCastle1<<"\t"<<minDisToCastle2<<"\t"<<minDisToCastle3<<"\t"<<minDisToCastle4<<endl;
					fout.close();
					return -2;
				}
			}
			////////////////////////////////
			/// End of second for loop ///
			////////////////////////////////
			DrawCastle(ct);
			DrawEnemies(enemies, numOfEnemy);
			t += 1;	// timer
	}
	//////////////////////////////
	//////End of while loop///////
	//////////////////////////////
	return 0;
}