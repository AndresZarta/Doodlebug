// NYU Tandon to Bridge Program Spring 2021
// Author: Andres Nicolas Zarta
// Professor: Daniel Katz
/* Description: Program that let's you visualize a predator-prey simulation using X's for predators and o's for prey.
 *
The goal for this Project is to create a simple two-dimensional predator-prey simulation. In this simulation the prey
are ants and the predators are doodlebugs. These critters live in a world composed of a 20 × 20 grid of cells. Only one
critter may occupy a cell at a time. The grid is  enclosed, so a critter is not allowed to move off the edges of the
world. Time is simulated in time steps. Each critter performs some action every time step.

The ants behave according to the following model:
Move. Every time step, randomly try to move up, down, left, or right. If the neighboring cell in the selected direction
is occupied or would move the ant off the grid, then the ant stays in the current cell.

Breed. If an ant survives for three time steps, then at the end of the time step (that is; after moving) the ant will
breed. This is simulated by creat-ing a new ant in an adjacent (up, down, left, or right) cell that is empty. If there
is no empty cell available, then no breeding occurs. Once an off-spring is produced, an ant cannot produce an offspring
until three more time steps have elapsed.

The doodlebugs behave according to the following model:
Move. Every time step, if there is an adjacent ant (up, down, left, or right), then the doodlebug will move to that cell
and eat the ant. Otherwise, the doodlebug moves according to the same rules as the ant. Note that a doodlebug cannot eat
other doodlebugs.

Breed. If a doodlebug survives for eight time steps, then at the end of the time step it will spawn off a new doodlebug
in the same manner as the ant.

Starve. If a doodlebug has not eaten an ant within the last three time steps, then at the end of the third time step it
will starve and die. The doodlebug should then be removed from the grid of cells.

During one turn, all the doodlebugs should move before the ants do.

Write a program to implement this simulation and draw the world using ASCII characters of “o” for an ant and “X” for a
doodlebug or "-" for an empty space. Create a class named Organism that encapsulates basic data common to both ants and
doodlebugs. This class should have a virtual function named move that is defined in the derived classes of Ant and
Doodlebug. You may need additional data structures to keep track of which critters have moved.

Initialize the world with 5 doodlebugs and 100 ants. After each time step, prompt the user to press Enter to move to the
next time step. You should see a cyclical pattern between the population of predators and prey, although random
perturbations may lead to the elimination of one or both species.
*/

// Last Changed: Jun 10, 2021


#include <iostream>
#include <vector>

using namespace std;

int ANT_BREED_CYCLE = 3;
char ANT_SHAPE = 'o';
string ANT_SPECIES = "Ant";
int DOODLEBUG_BREED_CYCLE = 8;
char DOODLEBUG_SHAPE = 'X';
string DOODLEBUG_SPECIES = "Doodlebug";
int DOODLEBUG_MAX_HUNGER = 3;
class Organism;
Organism* EMPTY_CELL = nullptr;
struct twoDCells;
twoDCells* OUT_OF_BOUNDS = nullptr;
char ANT = 'A';
char DOODLEBUG = 'D';
char EMPTY = 'E';


class twoDWorldOfPredatorsAndPreys;
struct twoDCells{ //This is a cell for a 2d world that contains an open spot
    // for an organism, a ptr to the world it belongs to, and pointers to its neighbors.
    twoDCells* northNeighbor = OUT_OF_BOUNDS;
    twoDCells* eastNeighbor = OUT_OF_BOUNDS;
    twoDCells* southNeighbor = OUT_OF_BOUNDS;
    twoDCells* westNeighbor = OUT_OF_BOUNDS;
    Organism* ptrToOrganism = EMPTY_CELL;
    twoDWorldOfPredatorsAndPreys* ptrToWorldOfOrganisms = nullptr;
    // Pointer to the address of the world the cell belongs to.
};

class Organism { //Base class for any organism.
    int age = 0; //Knows its age, species and shape,
    // whether it's alive, how long its breed cycle is, and a ptr to the cell it exists in.
    bool alive = true;

    string species;
    char shape;
    int breedCycle;

    twoDCells* cell;//This is the only way for the organism to have any awareness and ability to
    // learn about the world it inhabits.

public:
    Organism(string species, char shape, int breedCycle, twoDCells* cell)
    : species(species), shape(shape), breedCycle(breedCycle), cell(cell){};
    void increaseAge() {age++;}
    bool getAlive()const{return alive;}
    void setDead(){alive = false;}

    string getSpecies() {return species;}
    char getShape() const {return shape;}

    twoDCells* getCell() {return cell;}
    void setCell(twoDCells* newCell) {cell = newCell;}

    void breed();

    virtual void activate() = 0;
    virtual void move();
    void die();
    twoDCells* lookFor(char element);
    virtual ~Organism(){}
};

void Organism::move() {
    twoDCells* currentSpace = getCell();
    int randomDirection = (rand() % 4) + 1;
    twoDCells* possibleDirection = OUT_OF_BOUNDS;
    //If the random possibleDirection is valid (is not OUT OF BOUNDS) set it as a possible direction for moving.
    switch (randomDirection) {
        case 1:
            if (currentSpace->northNeighbor != OUT_OF_BOUNDS)
                possibleDirection = getCell()->northNeighbor;
            break;
        case 2:
            if (currentSpace->eastNeighbor != OUT_OF_BOUNDS)
                possibleDirection = getCell()->eastNeighbor;
            break;
        case 3:
            if (currentSpace->southNeighbor != OUT_OF_BOUNDS)
                possibleDirection = getCell()->southNeighbor;
            break;
        case 4:
            if (currentSpace->westNeighbor != OUT_OF_BOUNDS)
                possibleDirection = getCell()->westNeighbor;
            break;
    }

    if (possibleDirection != OUT_OF_BOUNDS){//If there is a possible direction.
        if (possibleDirection->ptrToOrganism == EMPTY_CELL){//If the possible direction has no organism/is empty.
            possibleDirection->ptrToOrganism = this;//Move there.
            setCell(possibleDirection);//Update your awareness of where you moved.
            currentSpace->ptrToOrganism = EMPTY_CELL;//Vacate the space you just left.
        }
    }

}

void Organism::die() {
    setDead(); //Updates the flag that says
    // dead.
    getCell() -> ptrToOrganism = EMPTY_CELL; //The cell it inhabited is now empty of organisms.
    //That organism is only pointed to by the pointer in the world's corresponding predator/prey list.
}


class Ant: public Organism{

public:
    Ant(twoDCells* cell) : Organism(ANT_SPECIES, ANT_SHAPE, ANT_BREED_CYCLE, cell){};
    virtual void activate();
    ~Ant(){}
};

void Ant::activate() {
    increaseAge(); //Age is now equal to the number of time steps it has activated.
    move();
    breed(); //It breeds every three time steps, when its age is a multiple of its breed cycle.
}

class Doodlebug: public Organism{
    void kill(twoDCells *targetCell);
    void starve();
    void increaseHunger() {hunger++;}
    int hunger = 0;

public:
    Doodlebug(twoDCells* cell) : Organism(DOODLEBUG_SPECIES, DOODLEBUG_SHAPE, DOODLEBUG_BREED_CYCLE, cell){};
    virtual void activate();
    virtual void move();
    ~Doodlebug(){}
};

void Doodlebug::kill(twoDCells *targetCell) {
    targetCell -> ptrToOrganism -> die();//Called with a target cell, sets the organism in that cell to die.
}

void Doodlebug::starve() {
    if (hunger == DOODLEBUG_MAX_HUNGER){ //When it has moved 3 times without eating an ant, it will die.
        die();
    }
}

void Doodlebug::activate(){
    increaseAge();//Age is now equal to the number of time steps it has activated.
    move(); //If it eats an ant, hunger is 0, if it doesn't hunger increases by one.
    breed(); //It breeds every eight time steps, when its age is a multiple of its breed cycle.
    starve();//It starves after three time steps without eating, when its hunger is 3.
}

void Doodlebug::move() {
    twoDCells* oldSpace = getCell();
    twoDCells* newSpace = lookFor(ANT); //Looks for an Ant in adjacent cells.
    if (newSpace != OUT_OF_BOUNDS){ //If it does find an Ant.
        kill(newSpace);//Kills it.
        newSpace->ptrToOrganism = this; //The new space cell now points to it.
        setCell(newSpace); //It recognizes the new space cell as its new location.
        oldSpace->ptrToOrganism = EMPTY_CELL; //It vacates the last cell.
        hunger = 0;//Resets its hunger.
    }
    else { //If it doesn't find an adjacent Ant.
        Organism::move();//Moves like a regular organism/ant.
        increaseHunger();
    }
}

class twoDWorldOfPredatorsAndPreys{
    int height;
    int width;
    int maxNumOfOrganisms = height * width;
    twoDCells** worldOfOrganisms; //This will be the pointer to the 2D array of cells
    vector <Organism *> preyList;
    vector <Organism *> predatorList;

public:
    twoDWorldOfPredatorsAndPreys(int height, int width) : height(height), width(width){generateEmptyWorldOfOrganisms();}
    void generateEmptyWorldOfOrganisms();
    void populateWithAntsAndDoodlebugs(int numberOfAnts, int numberOfDoodlebugs);
    void printVisibleWorld();
    void timeStep();
    friend char getCellContent(twoDCells* cell);
    //Functions related to predator/prey lists. The world's main way of activating the critters.
    void addToPredatorList(Organism* new_Organism) {predatorList.push_back(new_Organism);}
    void addToPreyList(Organism* new_Organism){preyList.push_back(new_Organism);}
    vector <Organism*> getPredatorList() {return predatorList;}
    vector <Organism*> getPreyList() {return preyList;}
    void removeFromPredatorList(int index) {predatorList.erase(predatorList.begin() + index);}
    void removeFromPreyList(int index) {preyList.erase(preyList.begin() + index);}
    void eraseWorldFromMemory();
    ~twoDWorldOfPredatorsAndPreys(){this->eraseWorldFromMemory();}
};

void twoDWorldOfPredatorsAndPreys::generateEmptyWorldOfOrganisms() {
    twoDCells** twoDWorldPtr = new twoDCells* [height]; //An array of Ptrs to Ptrs
    for (int i = 0; i < height; i++){
        twoDWorldPtr[i] = new twoDCells[width];
    }
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            twoDWorldPtr[i][j].ptrToOrganism = EMPTY_CELL; //All cells are initially empty of organisms.
            twoDWorldPtr[i][j].ptrToWorldOfOrganisms = this; //A pointer back to the world.
            //This is the only time we'll concern ourselves with X,Y coordinates. The world sets all of the cells
            //in the 2D array to point to its neighbors.

            if (i != 0) //If there is an adjacent cell in the 2D-array, set it as its neighbor, otherwise it's a border
                //and remains an OUT_OF_BOUNDS cell.
                twoDWorldPtr[i][j].northNeighbor = &twoDWorldPtr[i - 1][j];
            if (i != height - 1)
                twoDWorldPtr[i][j].southNeighbor = &twoDWorldPtr[i + 1][j];
            if (j != 0)
                twoDWorldPtr[i][j].westNeighbor = &twoDWorldPtr[i][j - 1];
            if (j != width - 1)
                twoDWorldPtr[i][j].eastNeighbor = &twoDWorldPtr[i][j + 1];
        }
    }
    worldOfOrganisms = twoDWorldPtr;
}

void twoDWorldOfPredatorsAndPreys::populateWithAntsAndDoodlebugs(int numberOfAnts, int numberOfDoodlebugs) {
    srand(time(0));
    if (maxNumOfOrganisms < numberOfAnts + numberOfDoodlebugs){
        cout << "The number of Ants + Doodlebugs is above the capacity for your world.";
        exit(1);
    }
    for (int ants = 1; ants <= numberOfAnts; ants++) {
        bool hasSpawned = false;
        while (!hasSpawned){ //It will keep trying to randomly find a viable spot to place an ant.
            int randomRow = rand() % height;
            int randomColumn = rand() % width;
            twoDCells *cellCandidate = &worldOfOrganisms[randomRow][randomColumn];
            twoDCells **ptrToCandidate = &cellCandidate;

            if (cellCandidate->ptrToOrganism == EMPTY_CELL){ //If it finds an empty spot for an ant.
                cellCandidate->ptrToOrganism = new Ant(*ptrToCandidate); //Creates a new ant.
                // The cell points to it now and it itself points to the cell.
                hasSpawned = true;
                Organism* temp = cellCandidate->ptrToOrganism;
                preyList.push_back(temp);//Adds a pointer to that Ant in the preyList.
            }
        }
    }
    for (int doodlebugs = 1; doodlebugs <= numberOfDoodlebugs; doodlebugs++) {
        bool hasSpawned = false;
        while (!hasSpawned){
            int randomRow = rand() % height;
            int randomColumn = rand() % width;
            twoDCells *cellCandidate = &worldOfOrganisms[randomRow][randomColumn];
            twoDCells **ptrToCandidate = &cellCandidate;
            if (cellCandidate->ptrToOrganism == EMPTY_CELL){//If it finds an empty spot for a doodlebug.
                cellCandidate->ptrToOrganism = new Doodlebug(*ptrToCandidate);//Creates a new doodlebug.
                // The cell points to it now and it itself points to the cell.
                hasSpawned = true;
                Organism* temp = cellCandidate->ptrToOrganism;
                predatorList.push_back(temp);//Adds a pointer to that doodlebug in the predatorList.
            }
        }
    }
}

void twoDWorldOfPredatorsAndPreys::printVisibleWorld() {
    for (int i = 0; i < height ; i++) {
        for (int j = 0; j < width; j++){
            if (worldOfOrganisms[i][j].ptrToOrganism == EMPTY_CELL)
                cout << "-\t";
            else
                cout << (worldOfOrganisms[i][j].ptrToOrganism)->getShape() << "\t";
        }
        cout << endl;
    }
}

char getCellContent(twoDCells *cell) {
    if (cell->ptrToOrganism == EMPTY_CELL)
        return EMPTY;
    else if (cell->ptrToOrganism->getSpecies() == DOODLEBUG_SPECIES)
        return DOODLEBUG;
    else if (cell->ptrToOrganism->getSpecies() == ANT_SPECIES)
        return ANT;
    else
        return 0;

}

twoDCells *Organism::lookFor(char element) { //A general purpose look-for function that looks for a specific char element
    //in adjacent cells, starting with a random direction. Right now it iss used to look for empty spots or ants.
    int temp = rand() % 4;

    int lookedAtAllDirections = 1;
    int currentDirection = temp + 1;

    while (lookedAtAllDirections <= 4){
        switch (currentDirection) {
            case 1://
                if (cell->northNeighbor == OUT_OF_BOUNDS){//If the cell is out of bounds, stop looking.
                    break;
                }
                else if (getCellContent(cell->northNeighbor) == element){//If the cell has the element you're looking
                    //for, return the ptr to cell.
                    return cell->northNeighbor;
                }
                else
                    break;
            case 2:
                if (cell->eastNeighbor == OUT_OF_BOUNDS){
                    break;
                }
                else if (getCellContent(cell->eastNeighbor) == element){
                    return cell->eastNeighbor;
                }
                else
                    break;
            case 3:
                if (cell->southNeighbor == OUT_OF_BOUNDS){
                    break;
                }
                else if (getCellContent(cell->southNeighbor)== element){
                    return cell->southNeighbor;
                }
                else
                    break;
            case 4:
                if (cell->westNeighbor == OUT_OF_BOUNDS){
                    break;
                }
                else if (getCellContent(cell->westNeighbor) == element){
                    return cell->westNeighbor;
                }
                else
                    break;
        }
        currentDirection++;
        if (currentDirection > 4)
            currentDirection = 1;
        lookedAtAllDirections++;
    }
    return OUT_OF_BOUNDS;
}

void Organism::breed() {
    if ((age >= breedCycle) && (age  % breedCycle == 0)){
        twoDCells* temp = lookFor(EMPTY); //Looks for an empty spot around it.
        if (temp != OUT_OF_BOUNDS){ //Only if it finds a viable empty spot.
            if (species == ANT_SPECIES){ //Breeds according to its species.
                temp->ptrToOrganism = new Ant(temp);
                Organism* ptrForList = temp->ptrToOrganism;
                cell->ptrToWorldOfOrganisms->addToPreyList(ptrForList);
            }
            if (species == DOODLEBUG_SPECIES){
                temp->ptrToOrganism = new Doodlebug(temp);
                Organism* ptrForList = temp->ptrToOrganism;
                cell->ptrToWorldOfOrganisms->addToPredatorList(ptrForList);
            }
        }
    }
}


void twoDWorldOfPredatorsAndPreys::timeStep() {
    int numOfDoodles = this->getPredatorList().size(); //Get the number of critters in the list at the start of the step.
    int numOfAnts = this->getPreyList().size();

    for (int i = 0; i < numOfDoodles; i++){ //Activate all Doodlebugs that started the round in the list.
        this->getPredatorList()[i]->activate();

    }
    for (int i = 0; i < numOfAnts; i++) {//Activate all ants that are still alive and started the round in the list.
        if (this->getPreyList()[i]->getAlive())
            this->getPreyList()[i]->activate();
    }

    for (int i = 0; i < this->getPredatorList().size(); i++) { //Iterate over the range of all predators.
        if (!this->getPredatorList()[i]->getAlive()){//If the predator is not alive.
            delete this->getPredatorList()[i];//delete the predator from memory.
            this->removeFromPredatorList(i);//remove it from the list.
            i--;
        }

    }
    for (int i = 0; i < this->getPreyList().size(); i++) {//Iterate over the range of all preys.
        if (!this->getPreyList()[i]->getAlive()){//If the prey is not alive.
            delete this->getPreyList()[i];//delete the prey from memory.
            this->removeFromPreyList(i);//remove it from the list.
            i--;
        }
    }

}

void twoDWorldOfPredatorsAndPreys::eraseWorldFromMemory() {

    int sizeOfPredatorList = this->getPredatorList().size();
    int sizeOfPreyList = this->getPreyList().size();
    for (int i = 0; i < sizeOfPredatorList; i++) {//Erase every remaining predator from memory.
        delete this->getPredatorList()[i];

    }
    for (int i = 0; i < sizeOfPreyList; i++) {//Erase every remaining prey from memory.
        delete this->getPreyList()[i];
    }

    for (int i = 0; i < height; i++){//Erase all columns.
        delete [] worldOfOrganisms[i];
    }
    delete [] worldOfOrganisms;//Erase rows.

}


int main() {

    twoDWorldOfPredatorsAndPreys world(20, 20);
    world.populateWithAntsAndDoodlebugs(100, 5);

    cout <<"Initial World" << endl;
    world.printVisibleWorld();

    char input;
    int counter = 1;
    cout << "\nPress ENTER to begin the simulation: ";
    cin.get(input);
    while (input == '\n'){
        world.timeStep();

        int numOfDoodles = world.getPredatorList().size();
        int numOfAnts = world.getPreyList().size();

        cout <<"\n//////////// Time Step " << counter << " ////////////" << endl;
        world.printVisibleWorld();
        cout <<"Number of Doodlebugs " << numOfDoodles << endl;
        cout <<"Number of Ants " << numOfAnts << endl;

        counter++;
        cout << "\nPress ENTER for the next step or input an alphanumeric key to stop the simulation:  ";
        cin.get(input);

    }

    return 0;
}
