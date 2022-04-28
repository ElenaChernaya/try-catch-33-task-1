#include <iostream>
#include <map>
#include <exception>
#include <algorithm>

// Переопределяем метод what() для проверки символов
class ValidCharacter: public std::exception
{
    const char* what() const noexcept override
    {
        return "Invalid character!";
    }
};

// Переопределяем метод what() для проверки наличия товара
class FoundProduct: public std::exception
{
    const char* what() const noexcept override
    {
        return "Product not found!";
    }
};


// Для проверки корректости ввода артикула
// Валидные символы: 0-9, a-z, A-Z, '_'
std::string valid_code(std::string& code)
{
    bool ok = true;
    for (auto i : code)
    {
        if ((i < '0' || i > 'z') || (i > '9' && i < 'A')
            || (i > 'Z' && i < 'a' && i != '_'))
            ok = false;
    }

    if (!ok) throw ValidCharacter();

    return code;
}


// Для проверки корректности количества товара
int valid_amount(int& num)
{
    if (num < 0) throw std::invalid_argument("Incorrect quantity");

    return num;
}


// Добавить товар в корзину
void add(std::map<std::string, int>& basket, std::string& product, int& amount)
{

// Если артикул некорректный, то выводится сообщение, иначе ищем товар с таким артикулом.
// Если не находим, то добавляем новый товар с этим артикулом.
// Если находим, то в этот товар просто добавляем кол-во штук этого товара

    try
    {
        // Если артикул соответствует артикулу товара в корзине
        if (auto pos = basket.find(valid_code(product)); pos !=
                                                         basket.end())
        {
            basket[pos->first] += valid_amount(amount);
        }

            // Иначе добавляем новый артикул и кол-во штук в корзину
        else {
            basket.insert(std::make_pair(valid_code(product),
                                         valid_amount(amount)));
        }
    }

    catch (const std::exception& ex)
    {
        std::cerr << "Error! " << ex.what() << std::endl;
    }

}


// Проверка на наличие товара в корзине
std::string Product(std::map<std::string, int>& basket, std::string& product, int& amount) {

    // Если товар не найден в корзине
    if (auto pos = basket.find(product); pos == basket.end())
    {
        throw FoundProduct();
    }

    // Если товар найден, но запрашиваемое кол-во штук, которое нужно удалить - превышает кол-во штук в корзине
    if (auto pos = basket.find(product); pos != basket.end())   {
        if (pos->second - amount < 0)
        {
            throw std::invalid_argument("Incorrect quantity");
        }
    }
    return product;
}


// Удалить товар из корзины
void remove (std::map<std::string, int>& basket, std::string& product, int& amount)
{
    try
    {
        auto pos = basket.find(Product(basket,product,amount));
        pos->second -= amount;

        if (pos->second == 0)
            basket.erase(pos);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}


// Вывод корзины в консоль
void printBasket(std::map<std::string, int>& basket)
{
    std::cout << "\n*** Current Cart Contents ***" <<
              std::endl;
    for (auto it = basket.begin(); it != basket.end();
         ++it)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
}



int main() {
    std::map<std::string, int> basket =
            {
                    {"TOILET_PAPER", 26},
                    {"Buckwheat", 213},
                    {"sugar007", 3}
            };

    std::string product;
    int amount;
    std::string choose = "unknown";

    std::cout << "\nChoose an action:\n";
    std::cin >> choose;


    while(choose != "exit")
    {
        while(choose == "add" || choose == "remove")
        {
            std::cout << "Enter the item number and quantity:\n";
            std::cin >> product >> amount;

            if (choose == "add")
            {
                add(basket, product, amount);
                printBasket(basket);
                break;
            }

            else if (choose == "remove")
            {
                remove(basket, product, amount);
                printBasket(basket);
                break;
            }
        }

        std::cout << "\nChoose an action:\n";
        std::cin >> choose;

        if (choose != "add" && choose != "remove" && choose != "exit")
        {
            std::cout << "You entered an invalid command!" << std::endl;
        }
    }
}