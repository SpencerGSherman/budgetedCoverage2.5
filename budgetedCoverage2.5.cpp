#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class Sensor {
public:
	Sensor() {
		xPos = rand() % 101;
		yPos = rand() % 101;
		cost = (float)rand() / (float)RAND_MAX * 10;
		radius = 5;
        isCovered = false;
        isPurchasable = true;
        sensorCoverage = 0;
        id = sensorCount;
        sensorCount++;
	}
    Sensor(int xMin, int xMax, int yMin, int yMax, int priceMin, int priceMax) {
        xPos = (rand() % (xMax + 1 - xMin)) + xMin;
        yPos = (rand() % (yMax + 1 - yMin)) + yMin;
        cost = ((float)rand() / (float)RAND_MAX * (priceMax - priceMin)) + priceMin + 0.01;
        radius = 5;
        isCovered = false;
        isPurchasable = true;
        sensorCoverage = 0;
        id = sensorCount;
        sensorCount++;
    }
    //~Sensor() {
    //    sensorCount--;
    //}
    bool operator<(const Sensor& sensor) const {
        return (cost < sensor.cost);
    }
    bool operator!= (const Sensor& otherSensor) {
        return (xPos != otherSensor.xPos || yPos != otherSensor.yPos);
    }

    static int sensorCount;
	int xPos, yPos, id, radius;
	float cost;
    bool isCovered, isPurchasable;
	vector<int> coverage;
    int sensorCoverage;
};

int Sensor::sensorCount = 0;
int tempMinimumCoverage = 0;
int tempRandomCoverage = 0;
int tempOptimizedCoverage = 0;
bool disableMenu = false;

void outputSensorPurchases(vector<Sensor>& city, vector<Sensor>& purchases) {
	ofstream MyFile("sensorPurchaseData.txt");
	for (int sensors = 0; sensors < city.size(); sensors++) {
		MyFile << "(" << city[sensors].xPos << ", " << city[sensors].yPos << ")";
	}
	for (int sensors = 0; sensors < purchases.size(); sensors++) {
		MyFile << "(" << purchases[sensors].xPos << ", " << purchases[sensors].yPos << ")";
	}
	MyFile.close();
}

vector<Sensor> sortByIncreasingCost(vector<Sensor> city) {
	sort(city.begin(), city.end(), [](const Sensor& current, const Sensor& next) {
		return current.cost < next.cost;
		});
	return city;
}

vector<Sensor> sortByDescreasingWeight(vector<Sensor> city) {
	sort(city.begin(), city.end(), [](const Sensor& current, const Sensor& next) {
		return (current.coverage.size() / current.cost) > (next.coverage.size() / next.cost);
		});
	return city;
}

void purachaseRandom(vector<Sensor> city, const float budget) {
    vector<Sensor> Purchases;
    bool purchasableNodes = true;
    float budgetRemaining = budget;

    int i = 0, e = 0;
    while (i < city.size()) {
        Sensor randomSensor = city[i];
        for (int sensors = 0; sensors < city.size(); sensors++) {
            if ((city[sensors].xPos - randomSensor.xPos) * (city[sensors].xPos - randomSensor.xPos) + (city[sensors].yPos - randomSensor.yPos) * (city[sensors].yPos - randomSensor.yPos) <= (randomSensor.radius * randomSensor.radius)) {
                city[i].sensorCoverage += 1;
            }
        }
        i++;
    }

    while (purchasableNodes) {
        int randIndex = rand() % (city.size());
        if (budgetRemaining - city[randIndex].cost < 0 || city[randIndex].isCovered) {
            city[randIndex].isPurchasable = false;
        }
        else {
            Purchases.push_back(city[randIndex]);
            for (int sensors = 0; sensors < city.size(); sensors++) {
                if ((city[sensors].xPos - city[randIndex].xPos) * (city[sensors].xPos - city[randIndex].xPos) + (city[sensors].yPos - city[randIndex].yPos) * (city[sensors].yPos - city[randIndex].yPos) <= city[randIndex].radius * city[randIndex].radius) {
                    city[sensors].isCovered = true;
                    city[sensors].isPurchasable = false;
                    city[randIndex].sensorCoverage++;
                }
            }
            budgetRemaining = budgetRemaining - city[randIndex].cost;
			if(!disableMenu){
				cout << "(" << city[randIndex].xPos << "," <<  city[randIndex].yPos << "){" <<  city[randIndex].sensorCoverage << "," << fixed << setprecision(2) <<  city[randIndex].cost << "," << ( city[randIndex].sensorCoverage /  city[randIndex].cost) << "}  ";
				cout << budgetRemaining << endl;
			}
        }

        purchasableNodes = false;
        for (int sensors = 0; sensors < city.size(); sensors++) {
            if (city[sensors].isPurchasable) {
                purchasableNodes = true;
                break;
            }
        }

        e = 0;
        while (e < city.size()) {
            for (int sensors = 0; sensors < city.size(); sensors++) {
                city[e].sensorCoverage = 0;
            }
            e++;
        }
        e = 0;
        while (e < city.size()) {
            Sensor randomSensor = city[e];
            for (int sensors = 0; sensors < city.size(); sensors++) {
                if ((city[sensors].xPos - randomSensor.xPos) * (city[sensors].xPos - randomSensor.xPos) + (city[sensors].yPos - randomSensor.yPos) * (city[sensors].yPos - randomSensor.yPos) <= (randomSensor.radius * randomSensor.radius)) {
                    if (city[sensors].isCovered == false) {
                        city[e].sensorCoverage = city[e].sensorCoverage + 1;
                    }
                }
            }
            e++;
        }
    }

    ofstream MyFile("randomPurchaseData.txt");
    for (int sensors = 0; sensors < city.size(); sensors++) {
        MyFile << "(" << city[sensors].xPos << ", " << city[sensors].yPos << ")";
    }
    for (int sensors = 0; sensors < Purchases.size(); sensors++) {
        MyFile << "(" << Purchases[sensors].xPos << ", " << Purchases[sensors].yPos << ")";
        tempRandomCoverage += Purchases[sensors].sensorCoverage;
    }
    MyFile.close();
	if(!disableMenu){cout << "\n";}
}

void purachaseMinimum(vector<Sensor> city, const float budget) {
    vector<Sensor> Purchases;
    bool purchasableNodes = true;
    float budgetRemaining = budget;

    vector<Sensor> costSortedCity = sortByIncreasingCost(city);

    int i = 0, e = 0;
	/*
    while (i < city.size()) {
        Sensor cheapSensor = costSortedCity[i];
        for (int sensors = 0; sensors < costSortedCity.size(); sensors++) {
            if ((costSortedCity[sensors].xPos - cheapSensor.xPos) * (costSortedCity[sensors].xPos - cheapSensor.xPos) + (costSortedCity[sensors].yPos - cheapSensor.yPos) * (costSortedCity[sensors].yPos - cheapSensor.yPos) <= (cheapSensor.radius * cheapSensor.radius)) {
                costSortedCity[i].sensorCoverage += 1;
            }
        }
        i++;
    }
	*/

    i = 0;
    while (purchasableNodes) {
        if (budgetRemaining - costSortedCity[i].cost < 0 || costSortedCity[i].isCovered) {
            costSortedCity[i].isPurchasable = false;
        }
        else {
            Purchases.push_back(costSortedCity[i]);
            for (int sensors = 0; sensors < city.size(); sensors++) {
                if ((costSortedCity[sensors].xPos - costSortedCity[i].xPos) * (costSortedCity[sensors].xPos - costSortedCity[i].xPos) + (costSortedCity[sensors].yPos - costSortedCity[i].yPos) * (costSortedCity[sensors].yPos - costSortedCity[i].yPos) <= (costSortedCity[i].radius * costSortedCity[i].radius)) {
                    costSortedCity[sensors].isCovered = true;
                    costSortedCity[sensors].isPurchasable = false;
                    costSortedCity[i].sensorCoverage += 1;
                }
            }
            budgetRemaining = budgetRemaining - costSortedCity[i].cost;
			if(!disableMenu){
				cout << "(" << costSortedCity[i].xPos << "," <<  costSortedCity[i].yPos << "){" <<  costSortedCity[i].sensorCoverage << "," << fixed << setprecision(2) <<  costSortedCity[i].cost << "," << ( costSortedCity[i].sensorCoverage /  costSortedCity[i].cost) << "}  ";
				cout << budgetRemaining << endl;
			}
        }

        purchasableNodes = false;
        for (int sensors = 0; sensors < costSortedCity.size(); sensors++) {
            if (costSortedCity[sensors].isPurchasable) {
                purchasableNodes = true;
                break;
            }
        }
        i++;

        e = 0;
        while (e < city.size()) {
            for (int sensors = 0; sensors < city.size(); sensors++) {
                costSortedCity[e].sensorCoverage = 0;
            }
            e++;
        }
        e = 0;
        while (e < city.size()) {
            Sensor cheapSensor = costSortedCity[e];
            for (int sensors = 0; sensors < city.size(); sensors++) {
                if ((costSortedCity[sensors].xPos - cheapSensor.xPos) * (costSortedCity[sensors].xPos - cheapSensor.xPos) + (costSortedCity[sensors].yPos - cheapSensor.yPos) * (costSortedCity[sensors].yPos - cheapSensor.yPos) <= (cheapSensor.radius * cheapSensor.radius)) {
                    if (costSortedCity[sensors].isCovered == false) {
                        costSortedCity[e].sensorCoverage = costSortedCity[e].sensorCoverage + 1;
                    }
                }
            }
            e++;
        }
    }

    ofstream MyFile("minimumPurchaseData.txt");
    for (int sensors = 0; sensors < city.size(); sensors++) {
        MyFile << "(" << costSortedCity[sensors].xPos << ", " << costSortedCity[sensors].yPos << ")";
    }
    for (int sensors = 0; sensors < Purchases.size(); sensors++) {
        MyFile << "(" << Purchases[sensors].xPos << ", " << Purchases[sensors].yPos << ")";
        tempMinimumCoverage += Purchases[sensors].sensorCoverage;
    }
    MyFile.close();
	if(!disableMenu){cout << "\n";}
}


void purachaseOptimized(vector<Sensor> city, float budget) {
    vector<Sensor> Purchases;
    bool purchasableNodes = true;
    int i = 0, e = 0;
    sort(city.begin(), city.end());
    while (i < city.size()) {
        Sensor optimizedSensor = city[i];
        for (int sensors = 0; sensors < city.size(); sensors++) {
            if ((city[sensors].xPos - optimizedSensor.xPos) * (city[sensors].xPos - optimizedSensor.xPos) + (city[sensors].yPos - optimizedSensor.yPos) * (city[sensors].yPos - optimizedSensor.yPos) <= (optimizedSensor.radius * optimizedSensor.radius)) {
                city[i].sensorCoverage += 1;
            }
        }
        i++;
    }
    sort(city.begin(), city.end(), [](const Sensor& lhs, const Sensor& rhs) {
        return ((lhs.sensorCoverage) / (lhs.cost)) > ((rhs.sensorCoverage) / (rhs.cost));
        });
    float budgetRemaining = budget;
    i = 0;
    while (i < city.size()) {
		if(budgetRemaining - city[i].cost < 0){
			i++;
		}else{
	        Sensor optimizedSensor = city[i];
	        if (optimizedSensor.isCovered) {
	            city[i].isPurchasable = false;
	        }
	        else {
	            Purchases.push_back(optimizedSensor);
	            for (int sensors = 0; sensors < city.size(); sensors++) {
	                if ((city[sensors].xPos - optimizedSensor.xPos) * (city[sensors].xPos - optimizedSensor.xPos) + (city[sensors].yPos - optimizedSensor.yPos) * (city[sensors].yPos - optimizedSensor.yPos) <= (optimizedSensor.radius * optimizedSensor.radius)) {
	                    city[sensors].isCovered = true;
	                    city[sensors].isPurchasable = false;
	                }
	            }
				budgetRemaining = budgetRemaining - optimizedSensor.cost;
				if(!disableMenu){
		            cout << "(" << optimizedSensor.xPos << "," << optimizedSensor.yPos << "){" << optimizedSensor.sensorCoverage << "," << fixed << setprecision(2) << optimizedSensor.cost << "," << (optimizedSensor.sensorCoverage / optimizedSensor.cost) << "}  ";
		            cout << budgetRemaining << endl;
				}
	        }
	        purchasableNodes = false;
	        for (int sensors = 0; sensors < city.size(); sensors++) {
	            if (city[sensors].isPurchasable) {
	                purchasableNodes = true;
	                break;
	            }
	        }
	        e = 0;
	        while (e < city.size()) {
	            for (int sensors = 0; sensors < city.size(); sensors++) {
	                city[e].sensorCoverage = 0;
	            }
	            e++;
	        }
	        e = 0;
	        while (e < city.size()) {
	            optimizedSensor = city[e];
	            for (int sensors = 0; sensors < city.size(); sensors++) {
	                if ((city[sensors].xPos - optimizedSensor.xPos) * (city[sensors].xPos - optimizedSensor.xPos) + (city[sensors].yPos - optimizedSensor.yPos) * (city[sensors].yPos - optimizedSensor.yPos) <= (optimizedSensor.radius * optimizedSensor.radius)) {
	                    if (city[sensors].isCovered == false) {
	                        city[e].sensorCoverage = city[e].sensorCoverage + 1;
	                    }
	                }
	            }
	            e++;
	        }
	        sort(city.begin(), city.end(), [](const Sensor& lhs, const Sensor& rhs) {
	            return ((lhs.sensorCoverage) / (lhs.cost)) > ((rhs.sensorCoverage) / (rhs.cost));
	            });
	        i++;
		}
    }

    ofstream MyFile("optimizedPurchaseData.txt");
    for (int sensors = 0; sensors < city.size(); sensors++) {
        MyFile << "(" << city[sensors].xPos << ", " << city[sensors].yPos << ")";
    }
    for (int sensors = 0; sensors < Purchases.size(); sensors++) {
        MyFile << "(" << Purchases[sensors].xPos << ", " << Purchases[sensors].yPos << ")";
        tempOptimizedCoverage += Purchases[sensors].sensorCoverage;
    }
    MyFile.close();
	if(!disableMenu){cout << "\n";}

}

void purachaseWeighted(vector<Sensor> city, const float budget) {
    vector<Sensor> Purchases;
    bool purchasableNodes = true;
    float budgetRemaining = budget;

    for (int node = 0; node < city.size(); node++) {
        for (int sensors = 0; sensors < city.size(); sensors++) {
            if ((city[sensors].xPos - city[node].xPos) * (city[sensors].xPos - city[node].xPos) + (city[sensors].yPos - city[node].yPos) * (city[sensors].yPos - city[node].yPos) <= (city[node].radius * city[node].radius)) {
                city[node].coverage.push_back(city[sensors].id);
            }
        }
    }

    vector<Sensor> weightSortedCity = sortByDescreasingWeight(city);

    int i = 0;
    while (purchasableNodes) {
		if(budgetRemaining - weightSortedCity[i].cost > 0){
			i++;
		}else{
        if (budgetRemaining - weightSortedCity[i].cost < 0 || weightSortedCity[i].isCovered) {
            weightSortedCity[i].isPurchasable = false;
        }
        else {
            Purchases.push_back(weightSortedCity[i]);
            sort(weightSortedCity[i].coverage.begin(), weightSortedCity[i].coverage.end());
            for (int sensors = i + 1; sensors < weightSortedCity.size(); sensors++) {
                sort(weightSortedCity[sensors].coverage.begin(), weightSortedCity[sensors].coverage.end());
                vector<int> commonId(weightSortedCity[sensors].coverage.size() + weightSortedCity[i].coverage.size());
                vector<int>::iterator iterator, st;

                iterator = set_intersection(weightSortedCity[sensors].coverage.begin(), weightSortedCity[sensors].coverage.end(), weightSortedCity[i].coverage.begin(), weightSortedCity[i].coverage.end(), commonId.begin());

                for (st = commonId.begin(); st != iterator; ++st) {
                    for (int removeSensors = 0; removeSensors < weightSortedCity[sensors].coverage.size(); removeSensors++) {
                        if (*st = weightSortedCity[sensors].coverage[removeSensors]) {
                            weightSortedCity[sensors].coverage.erase(weightSortedCity[sensors].coverage.begin() + removeSensors);
                        }
                    }
                }
            }
            budgetRemaining = budgetRemaining - weightSortedCity[i].cost;
        }

        weightSortedCity = sortByDescreasingWeight(weightSortedCity);

        purchasableNodes = false;
        for (int sensors = 0; sensors < weightSortedCity.size(); sensors++) {
            if (weightSortedCity[sensors].isPurchasable) {
                purchasableNodes = true;
                break;
            }
        }
        i++;
		}
    }
    ofstream MyFile("optimizedPurchaseData.txt");
    for (int sensors = 0; sensors < city.size(); sensors++) {
        MyFile << "(" << city[sensors].xPos << ", " << city[sensors].yPos << ")";
    }
    for (int sensors = 0; sensors < Purchases.size(); sensors++) {
        MyFile << "(" << Purchases[sensors].xPos << ", " << Purchases[sensors].yPos << ")";
    }
    MyFile.close();
} //Not used


int menu() {
    //Prompt user and return choice to main execution
    int ans;
    cout << "Please enter a option.\n";
    if (!disableMenu) {
        cout << "0. Toggle menu display\n";
        cout << "1. Lakeland\n";
        cout << "2. Miami\n";
        cout << "3. Boston\n";
        cout << "4. DesMoines\n";
        cout << "5. Luisville\n";
        cout << "6. Uniformland\n";
		cout << "7. Exit\n";

    }
	cout << "<<";
    cin >> ans;
    return ans;
}



int main() {
    srand(time(0));

    int ans;
    ans = menu();

    while (ans < 7) {
        if (ans == 0) {
            disableMenu = (!disableMenu) ? true : false;
        }
        if (ans == 1) {

            vector<Sensor> Lakeland;

            for (int sensors = 0; sensors < 25; sensors++) {
                Sensor boonies(0, 20, 0, 20, 0, 2);
                Lakeland.push_back(boonies);
                Sensor middleClass(25, 75, 10, 60, 1, 3);
                Lakeland.push_back(middleClass);
                Sensor upperEchelon(25, 75, 50, 90, 2, 5);
                Lakeland.push_back(upperEchelon);
                Sensor richyRich(80, 100, 80, 100, 7, 10);
                Lakeland.push_back(richyRich);
            }

            ofstream MyFile("LakelandCoveragePerformance.txt");
            for (int budget = 5; budget <= 50; budget += 5) {
                purachaseRandom(Lakeland, budget);
                purachaseMinimum(Lakeland, budget);
                purachaseOptimized(Lakeland, budget);
                MyFile << "(" << budget << ", " << tempRandomCoverage << ")";
                MyFile << "(" << budget << ", " << tempMinimumCoverage << ")";
                MyFile << "(" << budget << ", " << tempOptimizedCoverage << ")";
                tempMinimumCoverage = 0;
                tempRandomCoverage = 0;
                tempOptimizedCoverage = 0;
				if(!disableMenu){cout << "\n \n";}
            }
            MyFile.close();
        }
        if (ans == 2) {

            vector<Sensor> Miami;

            for (int sensors = 0; sensors < 25; sensors++) {
                Sensor inlandRiffRaffs(0, 20, 0, 100, 1, 2);
                Miami.push_back(inlandRiffRaffs);
                Sensor littleHavana(20, 40, 0, 100, 4, 6);
                Miami.push_back(littleHavana);
            }
            for (int sensors = 0; sensors < 30; sensors++) {
                Sensor downtown(40, 60, 0, 100, 8, 10);
                Miami.push_back(downtown);
            }
            for (int sensors = 0; sensors < 20; sensors++) {
                Sensor miamiBeach(60, 80, 0, 100, 9, 10);
                Miami.push_back(miamiBeach);
            }

            ofstream MyFile("MiamiCoveragePerformance.txt");
            for (int budget = 5; budget <= 50; budget += 5) {
                purachaseRandom(Miami, budget);
                purachaseMinimum(Miami, budget);
                purachaseOptimized(Miami, budget);
                MyFile << "(" << budget << ", " << tempRandomCoverage << ")";
                MyFile << "(" << budget << ", " << tempMinimumCoverage << ")";
                MyFile << "(" << budget << ", " << tempOptimizedCoverage << ")";
                tempMinimumCoverage = 0;
                tempRandomCoverage = 0;
                tempOptimizedCoverage = 0;
            }
            MyFile.close();
        }
        if (ans == 3) {

            vector<Sensor> Boston;

            for (int sensors = 0; sensors < 20; sensors++) {
                Sensor Downtown(50, 55, 50, 55, 8, 10);
                Boston.push_back(Downtown);
                Sensor FenwayKenmore(30, 40, 40, 50, 1, 3);
                Boston.push_back(FenwayKenmore);
                Sensor BackBay(40, 50, 45, 50, 5, 7);
                Boston.push_back(BackBay);
                Sensor SouthBoston(40, 55, 35, 45, 4, 5);
                Boston.push_back(SouthBoston);
                Sensor Charlestown(55, 65, 55, 65, 7, 9);
                Boston.push_back(Charlestown);
            }

            ofstream MyFile("BostonCoveragePerformance.txt");
            for (int budget = 5; budget <= 50; budget += 5) {
                purachaseRandom(Boston, budget);
                purachaseMinimum(Boston, budget);
                purachaseOptimized(Boston, budget);
                MyFile << "(" << budget << ", " << tempRandomCoverage << ")";
                MyFile << "(" << budget << ", " << tempMinimumCoverage << ")";
                MyFile << "(" << budget << ", " << tempOptimizedCoverage << ")";
                tempMinimumCoverage = 0;
                tempRandomCoverage = 0;
                tempOptimizedCoverage = 0;
            }
            MyFile.close();
        }
        if (ans == 4) {

            vector<Sensor> DesMoines;

            for (int sensors = 0; sensors < 12; sensors++) {
                Sensor Downtown(35, 65, 35, 65, 7, 9);
                DesMoines.push_back(Downtown);
            }
            for (int sensors = 0; sensors < 11; sensors++) {
                Sensor North(35, 65, 65, 95, 4, 6);
                DesMoines.push_back(North);
                Sensor South(35, 65, 5, 35, 4, 6);
                DesMoines.push_back(South);
                Sensor East(65, 95, 35, 65, 4, 6);
                DesMoines.push_back(East);
                Sensor West(5, 35, 35, 65, 4, 6);
                DesMoines.push_back(West);
                Sensor NorthWest(5, 35, 65, 95, 1, 3);
                DesMoines.push_back(NorthWest);
                Sensor NorthEast(65, 95, 65, 95, 1, 3);
                DesMoines.push_back(NorthEast);
                Sensor SouthWest(5, 35, 5, 35, 1, 3);
                DesMoines.push_back(SouthWest);
                Sensor SouthEast(65, 95, 5, 35, 1, 3);
                DesMoines.push_back(SouthEast);
            }

            ofstream MyFile("DesMoinesCoveragePerformance.txt");
            for (int budget = 5; budget <= 50; budget += 5) {
                purachaseRandom(DesMoines, budget);
                purachaseMinimum(DesMoines, budget);
                purachaseOptimized(DesMoines, budget);
                MyFile << "(" << budget << ", " << tempRandomCoverage << ")";
                MyFile << "(" << budget << ", " << tempMinimumCoverage << ")";
                MyFile << "(" << budget << ", " << tempOptimizedCoverage << ")";
                tempMinimumCoverage = 0;
                tempRandomCoverage = 0;
                tempOptimizedCoverage = 0;
            }
            MyFile.close();
        }
        if (ans == 5) {

            vector<Sensor> luisville;

            for (int sensors = 0; sensors < 33; sensors++) {
                Sensor fStudents(15, 40, 70, 95, 0, 2);
                luisville.push_back(fStudents);
            }
            for (int sensors = 0; sensors < 34; sensors++) {
                Sensor bStudents(40, 62, 32, 55, 4, 6);
                luisville.push_back(bStudents);
            }
            for (int sensors = 0; sensors < 33; sensors++) {
                Sensor aStudents(75, 90, 65, 95, 8, 10);
                luisville.push_back(aStudents);
            }
            ofstream MyFile("LuisvilleCoveragePerformance.txt");
            for (int budget = 5; budget <= 50; budget += 5) {
                purachaseRandom(luisville, budget);
                purachaseMinimum(luisville, budget);
                purachaseOptimized(luisville, budget);
                MyFile << "(" << budget << ", " << tempRandomCoverage << ")";
                MyFile << "(" << budget << ", " << tempMinimumCoverage << ")";
                MyFile << "(" << budget << ", " << tempOptimizedCoverage << ")";
                tempMinimumCoverage = 0;
                tempRandomCoverage = 0;
                tempOptimizedCoverage = 0;
            }
            MyFile.close();
        }
		if (ans == 6) {
			vector<Sensor>  Uniformland;

			for (int sensors = 0; sensors < 100; sensors++) {
			Sensor all(1, 99, 1, 99, 1, 9);
			Uniformland.push_back(all);
			}

			ofstream MyFile("UniformlandCoveragePerformance.txt");
            for (int budget = 5; budget <= 50; budget += 5) {
                purachaseRandom(Uniformland, budget);
                purachaseMinimum(Uniformland, budget);
                purachaseOptimized(Uniformland, budget);
                MyFile << "(" << budget << ", " << tempRandomCoverage << ")";
                MyFile << "(" << budget << ", " << tempMinimumCoverage << ")";
                MyFile << "(" << budget << ", " << tempOptimizedCoverage << ")";
                tempMinimumCoverage = 0;
                tempRandomCoverage = 0;
                tempOptimizedCoverage = 0;
            }
            MyFile.close();
        }

        ans = menu();
    }

	return 0;
}
