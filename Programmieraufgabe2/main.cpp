#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct node
{
    int key;
    struct node* left;
    struct node* right;
};

//Prints each node with balance factor & calculates min, max and sum & numOfNodes (which get used to calculate avg)
int printTreeStats(struct node* n, int height_left, int height_right, bool& isAVL, int& min, int& max, int& sum, int& numOfNodes) {
    //If n is a node
    if (n != nullptr) {
        //First, recursively calculate the left and right height through function return values
        height_left = printTreeStats(n->left, height_left, height_right, isAVL, min, max, sum, numOfNodes);
        height_right = printTreeStats(n->right, height_left, height_right, isAVL, min, max, sum, numOfNodes);

        //Calculates balance and prints if it is an AVL violation
        int balance = height_left - height_right;
        std::cout << "bal(" << n->key << ") = " << balance;

        if (balance > 1 || balance < -1) {
            isAVL = false;
            std::cout << " (AVL violation!)";
        }
        std::cout << std::endl;

        //Increments the number of nodes
        numOfNodes++;
        //Adds key to sum
        sum += n->key;

        //Sets the min & max to the key of the first node
        if (numOfNodes == 1) {
            min = n->key;
            max = n->key;
        }

        //If current key is smaller than min or bigger than max, set the min/max value as the key
        if (n->key < min)
            min = n->key;
        if (n->key > max)
            max = n->key;

        //Return the bigger height + 1 (+1 because the current node is a node that adds height too)
        if (height_left < height_right)
            return height_right + 1;
        return height_left + 1;
    }
    //If n is no node, return 0
    return 0;
}

//Recursively compares two trees, returns true if second tree was found, false if it was not
bool compareTrees(struct node* nodeOriginal, struct node* nodeToSearch, std::vector<int>& traversedNodes, bool& rootFound) {

    //Result of each recursion gets saved in this variable
    bool found = false;

    if (nodeOriginal != nullptr) { //If node of the main tree is not empty (if it is empty found stays false and gets returned)

        traversedNodes.push_back(nodeOriginal->key); //Saves the current node in the vector

        //If we have not found the first node of the second tree yet, continue to traverse the main tree until we find the root node
        if (!rootFound) {
            if (nodeOriginal->key < nodeToSearch->key) //Goes left if key is bigger
                found = compareTrees(nodeOriginal->left, nodeToSearch, traversedNodes, rootFound);
            else if (nodeOriginal->key > nodeToSearch->key)
                found = compareTrees(nodeOriginal->right, nodeToSearch, traversedNodes, rootFound);
        }
        //Once we found the root node, the first part of the recursion will stop, because we set rootFound to true
        //This means that in every following recursion, only this if matters
        if (nodeOriginal->key == nodeToSearch->key) {
            found = true; //Found gets set to true, but that might get changed back again to false if child nodes are not found
            rootFound = true;

            //If the node to the left of the second tree is not null, see if both left nodes are the same
            //(This will either succeed or fail the above if-check, if it fails found will stay false and get returned as false)
            if (nodeToSearch->left != nullptr) {
                found = compareTrees(nodeOriginal->left, nodeToSearch->left, traversedNodes, rootFound);
            }
            //If the right node is not null, do the same as above just on the right
            //Here you also need to check if found is still true (if the left node(s) are not found it is false)
            //If this additional check gets removed, the left could return false, but the right returns true and overwrites the false of the left!
            if (nodeToSearch->right != nullptr && found) {
                found = compareTrees(nodeOriginal->right, nodeToSearch->right, traversedNodes, rootFound);
            }
        }
    }

    //Returns the final state of found
    return found;
}

//Inserts a new node into the tree
struct node* insertElement(struct node* n, int key) {
    if (n == nullptr) {
        n = new struct node;
        n->key = key;
        n->left = nullptr;
        n->right = nullptr;
    }
    else {
        if (key > n->key) {
            n->left = insertElement(n->left, key);
        }
        else {
            n->right = insertElement(n->right, key);
        }
    }
    return n;
};

//Imports data from a file and creates a tree from it
struct node* importData(std::string fileName, struct node* root) {
    std::ifstream Inputfile(fileName);
    std::string line;

    if (Inputfile.is_open()) {
        while (getline(Inputfile, line)) {
            //Insert new nodes into BST
            //stoi casts String to int
            root = insertElement(root, stoi(line));
        }
        Inputfile.close();
    }
    else {
        std::cout << "Unable to open file" << std::endl;
    }

    return root;
}

int main()
{
    //TODO: These variables get taken from console input later, are hardcoded for now
    int numOfArguments = 2;
    std::string fileName = "Inputfile.txt";
    std::string searchName = "Searchfile.txt";

    struct node* root = nullptr; //Root of our primary tree

    root = importData(fileName, root); //Imports data from file and creates a tree from it

    //If we have 1 argument, we want to print its stats
    if (numOfArguments == 1) {
        bool isAVL = true; //Gets changed in printTreeStats() if a single AVL violation is found
        int min = 0;
        int max = 0;
        int sum = 0; //Used to calculate avg
        int numOfNodes = 0; //Used to calculate avg

        //Prints balance of each node + if it is an AVL violation
        printTreeStats(root, 0, 0, isAVL, min, max, sum, numOfNodes);

        float avg = static_cast<float>(sum) / static_cast<float>(numOfNodes); //Calculates avg as a float

        //Prints all final stats
        std::cout << "AVL: ";
        if (isAVL)
            std::cout << "yes";
        else
            std::cout << "no";
        std::cout << std::endl;

        std::cout << "min:" << min << ", max: " << max << ", avg: " << avg << std::endl;
    }
    //If we have more than 1 argument we want to perform a search
    else {
        struct node* searchRoot = nullptr; //Root of tree to search
        std::vector<int> traversedNodes; //List with traversed nodes to print later
        bool rootFound = false; //Helper variable for compareTrees function

        //Creates a tree from search data-file
        searchRoot = importData(searchName, searchRoot);

        //Bool if tree is only a single node (printing changes)
        //Note: Not needed for compareTrees()!
        bool isNoTree = searchRoot->left == nullptr && searchRoot->right == nullptr;

        //CompareTrees() returs true or false depending if tree was found
        if (compareTrees(root, searchRoot, traversedNodes, rootFound)) {
            //If tree only 1 node, prints info of single node + the traversed nodes
            if (isNoTree) {
                std::cout << searchRoot->key << " found";

                for (int i = 0; i < traversedNodes.size(); i++) {
                    if (i > 0)
                        std::cout << ",";
                    std::cout << " " << traversedNodes.at(i);
                }
            }
            else //If tree is an actual tree, prints simple found message
                std::cout << "Subtree found";
        }
        //If tree is not found, print according message
        else {
            if (isNoTree)
                std::cout << searchRoot->key << " not found!";
            else
                std::cout << "Subtree not found!";
        }

        std::cout << std::endl;
    }

    return 0;
}