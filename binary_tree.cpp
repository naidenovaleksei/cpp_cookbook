/**
 * Требуется построить бинарное дерево поиска, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root,
 * если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 * Выведите элементы в порядке level-order (по слоям, “в ширину”).
 */
#include <iostream>
#include <cassert>
#include <queue>

// Узел бинарного дерева
struct TreeNode {
    explicit TreeNode(int _value) : value(_value) {}

    int value = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

class Tree {
public:
    ~Tree();
    // Вывод значений дерева (обход в ширину)
    void Print() const;
    void Add(int value);

private:
    TreeNode* root = nullptr;

private:
    void delete_subtree(TreeNode* node);
};

Tree::~Tree() {
    delete_subtree(root);
}

void Tree::delete_subtree(TreeNode* node) {
    // Если нечего удалять, выходим
    if (!node) return;
    // Если есть - удаляем правый и левый дочерние узлы
    delete_subtree(node->left);
    delete_subtree(node->right);
    // Затем удаляем сам узел
    delete node;
}

void Tree::Print() const {
    // Если нечего показывать, выходим
    if (!root) return;
    // Для обхода в ширину создаем очередь "непройденных" узлов
    std::queue<TreeNode*> nodesQueue;
    // Добавляем корень
    nodesQueue.push(root);
    // Пока очередь не пустая
    while (!nodesQueue.empty()) {
        // Берем первый узел и отображаем его значение
        auto node = nodesQueue.front();
        std::cout << node->value << " ";
        // Вынимаем первый узел
        nodesQueue.pop();
        // Добавляем дочерние узлы в очередь
        if (node->left) nodesQueue.push(node->left);
        if (node->right) nodesQueue.push(node->right);
    }
}

void Tree::Add(int value) {
    // Если корень пустой - создаем и инициализируем узел в корне, выходим
    if (!root) {
        root = new TreeNode(value);
        return;
    }
    // Если корень непустой
    auto node = root;
    while (1) {
        // Проходим по (непустым) узлам
        assert(node);
        // Выбираем правый или левый дочерний узлы (в зависомости от значения и текущего узла)
        if (node->value <= value) {
            if (node->right) {
                // Если дочерний узел непустой - повторяем итерацию на нем
                node = node->right;
            } else {
                // Если нашли пустой дочерний узел - создаем узел и инициализируем узел
                node->right = new TreeNode(value);
                return;
            }
        } else {
            if (node->left) {
                // Если дочерний узел непустой - повторяем итерацию на нем
                node = node->left;
            } else {
                // Если нашли пустой дочерний узел - создаем узел и инициализируем узел
                node->left = new TreeNode(value);
                return;
            }
        }
    }
}


int main() {
    Tree tree;
    int n = 0;
    int value = 0;
    std::cin >> n;
    for (int i=0; i<n; ++i) {
        std::cin >> value;
        tree.Add(value);
    }
    tree.Print();
    return 0;
}