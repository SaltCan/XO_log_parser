#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <list>
#include <algorithm>
using namespace std;
string strtlvl = "starting level";
struct weapon {
	string weaponname = "CarPart_Gun_Machinegun_Starter";
	double damage = 0;
};
struct player {
	string nickname = NULL;
	list<weapon> playersarsenal;
};
string getmapname(string inp) {
	size_t mapnamepos = 1 + inp.find('\'');
	string tmp = "";
	while (inp[mapnamepos] != '\'') {
		tmp += inp[mapnamepos];
		mapnamepos++;
	}
	return tmp;
}
string getnickname(string inp) {
	size_t nicknamepos = 10 + inp.find("attacker: ");
	string tmp = "";
	while (inp[nicknamepos] != ' ') {
		tmp += inp[nicknamepos];
		nicknamepos++;
	}
	return tmp;
}
string getweaponname(string inp) {
	size_t gunnamepos = 1 + inp.find('\'');
	string tmp = "";
	while (inp[gunnamepos] != '\'') {
		tmp += inp[gunnamepos];
		gunnamepos++;
	}
	return tmp;
}
double getdamage(string inp) {
	size_t dmgpos = 8 + inp.find("damage: ");
	string tmp = "";
	while (inp[dmgpos] != ' ') {
		if (inp[dmgpos] == '.')
			tmp += ',';
		else
			tmp += inp[dmgpos];
		dmgpos++;
	}
	return stod(tmp);
}
list<player>::iterator SearchInPlayerlist(list<player> &playerlist, string playername) {
	for (list<player>::iterator it = playerlist.begin(); it != playerlist.end(); it++) {
		if (it->nickname == playername) {
			return it;
		}
	}
	return playerlist.end();
}
list<weapon>::iterator SearchInArsenal(list<weapon>::iterator start, list<weapon>::iterator finish, string gunname) {
	for (list<weapon>::iterator it = start; it != finish; it++) {
		if (it->weaponname == gunname) {
			return it;
		}
	}
	return finish;
}
void AccountDmg(list<player> &playerlist, string playername, string gunname, double damage) {
	list<player>::iterator thatguy = SearchInPlayerlist(playerlist, playername);
	if (thatguy == playerlist.end()) {
		playerlist.push_back(player{ playername });
		thatguy = playerlist.end();
		thatguy--;
		//cout << thatguy->nickname << endl;
	}
	list<weapon>::iterator thatgun = SearchInArsenal(thatguy->playersarsenal.begin(), thatguy->playersarsenal.end(), gunname);
	if (thatgun == thatguy->playersarsenal.end()) {
		thatguy->playersarsenal.push_back({ gunname,damage });
	} else {
		thatgun->damage += damage;
	}
}
int main() {
	setlocale(LC_ALL, "Russian");
	ifstream fin;
	ofstream fout;
	fin.open("combat.log");
	fout.open("output.txt");
	string buffer;
	for (int i = 0; i < 4; i++)
		getline(fin, buffer);
	while (!fin.eof()) {
		getline(fin, buffer);
		size_t tmp1 = buffer.find("starting level");
		if ((buffer.find("levels/maps/hangar") == string::npos) && (buffer.find("levels/maps/red_rocks_territory") == string::npos)) {  //начало боя
			if (tmp1 != string::npos) {
				fout << "Бой на карте " << getmapname(buffer);
				cout << "Бой на карте " << getmapname(buffer);
				if (buffer.find("AssaultBestOf3") != string::npos)
					cout << " (КВ)" << endl;
				else
					cout << " (Рандом)" << endl;
				list<player> newplayerlist;

				while ((buffer.size() != 0) && (!fin.eof())) {
					getline(fin, buffer);
					if (buffer.find("Damage.") != string::npos) {
						string nickname = getnickname(buffer);
						string gunnm = getweaponname(buffer);
						double damage = getdamage(buffer);
						//fout << "  Игрок " << nickname << " Нанёс " << damage << " едениц урона оружием " << gunnm << endl;
						AccountDmg(newplayerlist, nickname, gunnm, damage);
					}
					if (buffer.size() == 0) {

						for (list<player>::iterator it1 = newplayerlist.begin(); it1 != newplayerlist.end(); it1++) {
							cout << '\t' << it1->nickname << " :" << endl;
							fout << '\t' << it1->nickname << " :" << endl;
							for (list<weapon>::iterator it2 = it1->playersarsenal.begin(); it2 != it1->playersarsenal.end(); it2++) {
								cout << "\t\t" << it2->weaponname << ": " << it2->damage << " dmg" << endl;
								fout << "\t\t" << it2->weaponname << ": " << it2->damage << " dmg" << endl;
							}
						}
					}
				}
			}
		}
	}
	return 0;
	system("pause");
}
