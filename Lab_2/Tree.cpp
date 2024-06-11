#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>

bool inline  operator > (const std::string& left_number, const std::string& right_number) {
    if (left_number.size() == right_number.size()) {
        for (int i = 0; i < left_number.size(); ++i) {
            if (left_number[i] != right_number[i]) {
                return left_number[i] > right_number[i];
            }
        }
        return false;
    }
    if (left_number.size() < right_number.size()) {
        return false;
    }
    else {
        return true;
    }
}

bool inline operator < (const std::string& left_number, const std::string& right_number) {
    if (left_number.size() < right_number.size()) {
        return true;
    }
    else if (left_number.size() > right_number.size()) {
        return false;
    }
    else {
        for (int i = 0; i < left_number.size(); ++i) {
            if (left_number[i] != right_number[i]) {
                return left_number[i] < right_number[i];
            }
        }
        return false;
    }
}

bool inline operator >= (const std::string& left_number, const  std::string& right_number) {
    return !(left_number < right_number);
}

bool inline operator <= (const std::string& left_number, const  std::string& right_number) {
    return !(left_number > right_number);
}

bool inline operator == (const std::string& left_number, const std::string& right_number) {
    if (left_number.size() != right_number.size()) {
        return false;
    }
    for (size_t i = 0; i < left_number.size(); ++i) {
        if (left_number[i] != right_number[i]) {
            return false;
        }
    }
    return true;
}

class node;
using nodeptr = node*;

struct pair {
    nodeptr first;
    nodeptr second;
};

struct item {
    std::string key;
    unsigned long long value;
};

class node {
    item storage;
    int y;
    nodeptr left_branch;
    nodeptr right_branch;

    explicit node(std::string& key, unsigned long long value) {
        storage.key = key;
        storage.key.reserve(256);
        storage.value = value;
        y = rand();
        left_branch = nullptr;
        right_branch = nullptr;
    }


    void traverse_helper(nodeptr root) const {
        if (!root) {
            return;
        }
        traverse_helper(root->left_branch);
        std::cout << " " << root->storage.key << " " << root->storage.value << '\n';
        traverse_helper(root->right_branch);
    }




    friend class DecartTree;
};

class DecartTree {
    nodeptr root;
public:

    DecartTree() : root(nullptr) {}

    void traverse() const {
        if (root != nullptr) {
            root->traverse_helper(root);
        }
    }

    bool inserted(const std::string& key) const {
        return inserted_helper(root, key);
    }

    void findValue(const std::string& key) const {
        findValue_helper(root, key);
    }

    void insert(std::string& key, unsigned long long value) {
        root = insert_helper(root, key, value);
    }

    void remove(std::string key) {
        root = remove_helper(root, key);
    }

    void deleteTree() {
        deleteTree_helper(root);
    }

    void saveTreeToFile(const std::string& filename) {
        std::ofstream out(filename, std::ios::binary);
        if (!out) {
            std::cerr << "Cannot open file for writing: " << filename << std::endl;
            return;
        }
        saveNode(out, root);
        out.close();
    }

    void loadTreeFromFile(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) {
            std::cerr << "Cannot open file for reading: " << filename << std::endl;
            return;
        }
        deleteTree(); 
        root = loadNode(in);
        in.close();
    }

private:
    bool inserted_helper(nodeptr current, const std::string& key) const {
        if (!current) {
            return false;
        }
        if (current->storage.key == key) {
            return true;
        }
        if (key > current->storage.key) {
            return inserted_helper(current->right_branch, key);
        }
        return inserted_helper(current->left_branch, key);
    }

    void findValue_helper(nodeptr current, const std::string& key) const {
        if (!current) {
            std::cout << "NoSuchWord" << '\n';
            return;
        }
        if (current->storage.key == key) {
            std::cout << "OK: " << current->storage.value << '\n';
            return;
        }
        if (key > current->storage.key) {
            findValue_helper(current->right_branch, key);
        }
        else {
            findValue_helper(current->left_branch, key);
        }
    }

    pair split(node* root, std::string key) {
        if (!root) {
            return { nullptr, nullptr };
        }
        if (root->storage.key <= key) {
            auto res = split(root->right_branch, key);
            root->right_branch = res.first;
            return { root,res.second };
        }
        else {
            auto res = split(root->left_branch, key);
            root->left_branch = res.second;
            return { res.first,root };
        }
    }

    node* merge(node* root1, node* root2) {
        if (!root1) {
            return root2;
        }
        if (!root2) {
            return root1;
        }
        if (root1->y <= root2->y) {
            root1->right_branch = merge(root1->right_branch, root2);
            return root1;
        }
        else {
            root2->left_branch = merge(root1, root2->left_branch);
            return root2;
        }
    }

    node* insert_helper(node* root, std::string& key, unsigned long long value) {
        if (inserted(key) == true) {
            std::cout << "Exist" << '\n';
            return root;
        }
        auto res = split(root, key);
        node* newnode = new node(key, value);
        std::cout << "OK" << '\n';
        return merge(merge(res.first, newnode), res.second);
    }

    node* remove_helper(node* root, std::string& key) {
        if (inserted(key) == false) {
            std::cout << "NoSuchWord" << '\n';
            return root;
        }
        std::string key_minus_1 = key;
        key_minus_1[key_minus_1.size() - 1] = key_minus_1[key_minus_1.size() - 1] - 1;
        auto res1 = split(root, key);
        auto res2 = split(res1.first, key_minus_1);
        delete res2.second;
        std::cout << "OK" << std::endl;
        return merge(res2.first, res1.second);
    }

    void deleteTree_helper(nodeptr root) {
        if (root) {
            deleteTree_helper(root->left_branch);
            deleteTree_helper(root->right_branch);
            delete root;
        }
    }

    void saveNode(std::ofstream& out, nodeptr node) {
        if (!node) {
            out.write(reinterpret_cast<const char*>(&node), sizeof(node)); 
            return;
        }
        size_t key_size = node->storage.key.size(); 
        out.write(reinterpret_cast<const char*>(&key_size), sizeof(size_t)); 
        out.write(node->storage.key.data(), node->storage.key.size());
        out.write(reinterpret_cast<const char*>(&node->storage.value), sizeof(unsigned long long));
        saveNode(out, node->left_branch);
        saveNode(out, node->right_branch);
    }

    nodeptr loadNode(std::ifstream& in) {
        size_t key_size;
        in.read(reinterpret_cast<char*>(&key_size), sizeof(size_t));
        if (in.gcount() == 0) return nullptr; 

        std::string key;
        key.resize(key_size);
        in.read(&key[0], key_size);
        unsigned long long value;
        in.read(reinterpret_cast<char*>(&value), sizeof(unsigned long long));

        nodeptr newNode = new node(key, value);
        newNode->left_branch = loadNode(in);
        newNode->right_branch = loadNode(in);
        return newNode;
    }

};



int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string action, key;
    unsigned long long value;
    DecartTree tree;
    try {
        while (std::cin >> action) {

            for (size_t i = 0; i < action.size(); ++i) {
                action[i] = tolower(action[i]);
            }

            if (action == "+") {
                std::cin >> key >> value;
                for (size_t i = 0; i < key.size(); ++i) {
                    key[i] = tolower(key[i]);
                }
                tree.insert(key, value);
            }
            else if (action == "-") {
                std::cin >> key;
                for (size_t i = 0; i < key.size(); ++i) {
                    key[i] = tolower(key[i]);
                }
                tree.remove(key);
            }
            else if (action == "!") {
                std::string underaction;
                std::cin >> underaction;
                for (size_t i = 0; i < underaction.size(); ++i) {
                    underaction[i] = tolower(underaction[i]);
                }
                std::string path;
                if (underaction == "save") {
                    std::cin >> path;
                    tree.saveTreeToFile(path);
                }
                else if (underaction == "load") {
                    std::cin >> path;
                    tree.loadTreeFromFile(path);
                }
            }
            else {
                for (size_t i = 0; i < action.size(); ++i) {
                    action[i] = tolower(action[i]);
                }
                tree.findValue(action);
            }
        }
    }
    catch (const std::exception& except) {
        std::cout << "failed: " << except.what() << std::endl;
    }

    tree.deleteTree();
}
