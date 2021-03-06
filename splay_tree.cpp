/**
 * Splay-дерево.
 * Множество натуральных чисел постоянно меняется. Элементы в нем добавляются и удаляются по одному.
 * Вычислите указанные порядковые статистики после каждого добавления или удаления.
 * Требуемая скорость выполнения запроса - O(log n) амортизированно.
 * В реализации используйте сплей-деревья.
 */
#include <iostream>
#include <cassert>
#include <stack>

// Узел сплей дерева
struct TreeNode {
    explicit TreeNode(int _value, TreeNode* _parent) : value(_value), parent(_parent) {}

    int value = 0;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode* parent;
    int size = 0;
};

// Сплей дерево с расчетом порядковых статистик
class Tree {
public:
    Tree() {};
    ~Tree();
    Tree(const Tree &other);
    Tree& operator=(const Tree &other) ;
    Tree(Tree&& other) noexcept;
    Tree& operator=(Tree&& other) noexcept;

    // Добавить элемент
    void Add(int value);
    // Убрать элемент
    void Delete(int value);
    // Посчитать k-ю порядковую статистику
    int OrderStat(int k) const;

private:
    TreeNode* root = nullptr;

private:
    // Поиск элемента по дереву (возвращает узел с заданным числом или его родителя)
    TreeNode* find(int value, bool increment);
    // Операция сплей (поднять узел наверх)
    void splay(TreeNode* node);
    // Операции для поднятия узла наверх
    void zig(TreeNode* node);
    void zigzig(TreeNode* node);
    void zigzag(TreeNode* node);
    // Малые повороты относительно узла
    TreeNode* rotateLeft(TreeNode* root);
    TreeNode* rotateRight(TreeNode* root);
    // Устанавливаем связь между новым родителем и узлом
    void setLeftChild(TreeNode* new_parent, TreeNode* new_child);
    void setRightChild(TreeNode* new_parent, TreeNode* new_child);
    // Установка нового корня
    void setRoot(TreeNode* node);
    // Удаление узла и его потомков
    static void deleteSubtree(TreeNode* node);
    static TreeNode* copySubtree(TreeNode* other, TreeNode* parent);
    // Максимальный элемент дерева (начиная с данного узла)
    static TreeNode* max(TreeNode* node);
    // Склейка с деревом c бОльшими значениями
    void mergeRight(TreeNode* mergedRoot);
};

Tree::~Tree() {
    deleteSubtree(root);
}

Tree::Tree(const Tree &other) {
    root = copySubtree(other.root, nullptr);
}

Tree& Tree::operator=(const Tree &other) {
    if (this == &other)
        return *this;
    auto tmp_root = copySubtree(other.root, nullptr);
    deleteSubtree(root);
    root = tmp_root;
    return *this;
}

Tree::Tree(Tree&& other) noexcept {
    root = other.root;
    other.root = nullptr;
}

Tree& Tree::operator=(Tree&& other) noexcept {
    if (this == &other)
        return *this;
    deleteSubtree(root);
    root = other.root;
    other.root = nullptr;
    return *this;
}

void Tree::setRoot(TreeNode* node) {
    root = node;
    node->parent = nullptr;
}

TreeNode* Tree::find(int value, bool increment) {
    if (!root) return nullptr;
    // Начинаем с корня
    auto node = root;
    while (true) {
        assert(node);
        // Обновляем размер поддеревьев
        if (increment) ++node->size;
        // Если значение узла равно заданному, возвращаем узел
        if (node->value == value) {
            return node;
            // Иначе спускаемся вправо или влево в зависимости от значения в узле
        } else if (node->value <= value) {
            if (node->right) {
                node = node->right;
            } else {
                // Если нужного узла нет - возвращаем родителя
                return node;
            }
        } else {
            if (node->left) {
                node = node->left;
            } else {
                // Если нужного узла нет - возвращаем родителя
                return node;
            }
        }
    }
}

void Tree::deleteSubtree(TreeNode* node) {
    if (!node) return;
    deleteSubtree(node->left);
    deleteSubtree(node->right);
    delete node;
}

TreeNode* Tree::copySubtree(TreeNode* other, TreeNode* parent) {
    if (!other) return nullptr;
    TreeNode* node = new TreeNode(other->value, parent);
    node->left = copySubtree(other->left, node);
    node->right = copySubtree(other->right, node);
    return node;
}

// Поиск k-й статистики
int Tree::OrderStat(int k) const {
    assert(root);
    auto node = root;
    while (node) {
        // Смотри размер левого поддерева
        auto nodes_k = node->left ? node->left->size + 1 : 0;
        if (nodes_k == k) {
            // Если равен, то искомая статистика в текущем узле
            return node->value;
        } else if (nodes_k > k) {
            // Если меньше, спускаемся в левый узел
            node = node->left;
        } else {
            // Если меньше, спускаемся в правый узел и корректируем статистику k
            node = node->right;
            k -= nodes_k + 1;
        }
    }
}

void Tree::Delete(int value) {
    // Находим нужный узел (считаем что такой элемент есть)
    TreeNode* node = find(value, false);
    assert(node);
    // Поднимаемм узел в корень
    splay(node);
    auto left = node->left;
    auto right = node->right;
    // Если есть левый подузел, то ставим его корнем
    if (left) {
        setRoot(left);
        // а правый - склеиваем с новым корнем
        if (right) mergeRight(right);
    } else {
        // Если левого подузла нет, то ставим правый подузел корнем
        setRoot(right);
    }
    delete node;
    // Обновляем размер поддерева
    root->size = (root->left ? 1 + root->left->size : 0) + (root->right ? 1 + root->right->size : 0);
}

void Tree::Add(int value) {
    if (!root) {
        setRoot(new TreeNode(value, nullptr));
        return;
    }
    // Находим нужный элемент (точнее его родителя) + инкрементируем все узлы пути
    TreeNode* node = find(value, true);
    assert(node->value != value);
    // Если значение меньше значения узла, создаем левый подузел с заданным числом
    if (node->value < value) {
        assert(!node->right);
        node->right = new TreeNode(value, node);
        splay(node->right);
    } else {
        assert(!node->left);
        node->left = new TreeNode(value, node);
        splay(node->left);
    }
}

void Tree::mergeRight(TreeNode* mergedRoot) {
    auto maxLeftNode = max(root);
    splay(maxLeftNode);
    assert(!root->right);
    setRightChild(root, mergedRoot);
}

void Tree::zig(TreeNode* node) {
    assert(node && node->parent);
    TreeNode* p_node = node->parent;
    // Делаем правый или левый поворот относительно родителя
    if (node == p_node->left) rotateRight(p_node);
    if (node == p_node->right) rotateLeft(p_node);
    // Обновляем размер поддеревьев
    node->size = p_node->size;
    p_node->size = (p_node->left ? 1 + p_node->left->size : 0) + (p_node->right ? 1 + p_node->right->size : 0);
}

void Tree::zigzig(TreeNode* node) {
    assert(node && node->parent);
    TreeNode* p_node = node->parent;
    // Суть сплей-дерева: сначала выполняем ziq родителя
    zig(p_node);
    // а только потом - самого узла
    zig(node);
}

void Tree::zigzag(TreeNode* node) {
    assert(node);
    // Последовательно поднимаем узел 2 раза (поворот в одну, потом в другую сторону)
    zig(node);
    zig(node);
}

void Tree::splay(TreeNode* node) {
    // Пока не подняли
    while (true) {
        assert(node);
        // Если узел - корень, все готово
        if (node == root) {
            return;
        }
        auto parent = node->parent;
        assert(parent);
        // Если родитель - корень, делаем зиг
        if (parent == root) {
            zig(node);
            return;
        }
        auto gparent = parent->parent;
        assert(gparent);
        // Если узел и родитель - оба левые или оба правые потомки, то делаем зиг-зиг
        if ((node == parent->left) == (parent == gparent->left)) {
            zigzig(node);
        } else {
            // иначе делаем зиг-заг
            zigzag(node);
        }
    }
}

void Tree::setLeftChild(TreeNode* new_parent, TreeNode* new_child) {
    // 1) Новый левый помок
    new_parent->left = new_child;
    if (!new_child) return;
    // 2) Установили родителя
    new_child->parent = new_parent;
    // 3) Если нужно, поменяли корень
    if (new_child == root) setRoot(new_parent);
}

void Tree::setRightChild(TreeNode* new_parent, TreeNode* new_child) {
    // 1) Новый правый помок
    new_parent->right = new_child;
    if (!new_child) return;
    // 2) Установили родителя
    new_child->parent = new_parent;
    // 3) Если нужно, поменяли корень
    if (new_child == root) setRoot(new_parent);
}

// Правый поворот
TreeNode* Tree::rotateRight(TreeNode* root) {
    assert(root);
    assert(root->left);
    // Новый корень - левый потомок
    TreeNode* new_root = root->left;
    // Установили для старого корня нового левого потомка
    setLeftChild(root, new_root->right);
    // Если есть родитель корня, нужно задать ему нового потомка - новый корень
    auto parent = root->parent;
    if (parent) {
        if (root == parent->left) setLeftChild(parent, new_root);
        if (root == parent->right) setRightChild(parent, new_root);
    }
    // Новому корню установили правого потомка - старый корень
    setRightChild(new_root, root);
    return new_root;
}

// Левый поворот
TreeNode* Tree::rotateLeft(TreeNode* root) {
    assert(root);
    assert(root->right);
    // Новый корень - правый потомок
    TreeNode* new_root = root->right;
    // Установили для старого корня нового правого потомка
    setRightChild(root, new_root->left);
    // Если есть родитель корня, нужно задать ему нового потомка - новый корень
    auto parent = root->parent;
    if (parent) {
        if (root == parent->left) setLeftChild(parent, new_root);
        if (root == parent->right) setRightChild(parent, new_root);
    }
    // Новому корню установили левого потомка - старый корень
    setLeftChild(new_root, root);
    return new_root;
}

TreeNode* Tree::max(TreeNode* node) {
    assert(node);
    // Спускаемся по правым потомкам до конца
    while (node->right) {
        node = node->right;
    }
    return node;
}

int main() {
    Tree tree;
    int n = 0;
    int value = 0;
    int k = 0;
    std::cin >> n;
    for (int i=0; i<n; ++i) {
        std::cin >> value >> k;
        if (value > 0)
            tree.Add(value);
        if (value < 0)
            tree.Delete(-value);
        std::cout << tree.OrderStat(k) << std::endl;
    }
    return 0;
}
