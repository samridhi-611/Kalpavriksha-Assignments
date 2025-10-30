#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50

typedef struct {
    int id;
    char name[NAME_LEN];
    float price;
    int quantity;
} Product;

void addProduct(Product **inventory, int *count) {
    *inventory = (Product *)realloc(*inventory, (*count + 1) * sizeof(Product));
    if (!*inventory) {
        printf("Memory reallocation failed.\n");
        return;
    }

    Product *newProduct = &(*inventory)[*count];
    printf("Enter Product ID (1–10000): ");
    if (scanf("%d", &newProduct->id) != 1 || newProduct->id < 1 || newProduct->id > 10000) {
        printf("Invalid Product ID. Please enter a number between 1 and 10000.\n");
        while (getchar() != '\n');
        return;
    }
    getchar();

    printf("Enter Product Name: ");
    fgets(newProduct->name, NAME_LEN, stdin);
    newProduct->name[strcspn(newProduct->name, "\n")] = '\0';
    if (strlen(newProduct->name) == 0) {
        printf("Product name cannot be empty.\n");
        return;
    }

    printf("Enter Product Price: ");
    if (scanf("%f", &newProduct->price) != 1 || newProduct->price <= 0) {
        printf("Invalid price entered.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter Product Quantity: ");
    if (scanf("%d", &newProduct->quantity) != 1 || newProduct->quantity < 0) {
        printf("Invalid quantity entered.\n");
        while (getchar() != '\n');
        return;
    }

    (*count)++;
    printf("Product added successfully!\n");
}

void viewProducts(Product *inventory, int count) {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }

    printf("\n| ID | Name | Quantity | Price |\n");
    printf("----------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("| %d | %s | %d | %.2f |\n",
               inventory[i].id,
               inventory[i].name,
               inventory[i].quantity,
               inventory[i].price);
    }
    printf("----------------------------------\n");
}

void updateQuantity(Product *inventory, int count) {
    int targetId, updatedQuantity;
    printf("Enter Product ID to update quantity: ");
    if (scanf("%d", &targetId) != 1) {
        printf("Invalid Product ID.\n");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < count; i++) {
        if (inventory[i].id == targetId) {
            printf("Enter new Quantity: ");
            if (scanf("%d", &updatedQuantity) != 1 || updatedQuantity < 0) {
                printf("Invalid quantity entered.\n");
                while (getchar() != '\n');
                return;
            }
            inventory[i].quantity = updatedQuantity;
            printf("Quantity updated successfully!\n");
            return;
        }
    }
    printf("Product ID not found.\n");
}

void searchByID(Product *inventory, int count) {
    int targetId;
    printf("Enter Product ID to search: ");
    if (scanf("%d", &targetId) != 1) {
        printf("Invalid Product ID.\n");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < count; i++) {
        if (inventory[i].id == targetId) {
            printf("\n| ID | Name | Quantity | Price |\n");
            printf("----------------------------------\n");
            printf("| %d | %s | %d | %.2f |\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].quantity,
                   inventory[i].price);
            printf("----------------------------------\n");
            return;
        }
    }
    printf("Product not found.\n");
}

void searchByName(Product *inventory, int count) {
    char searchTerm[NAME_LEN];
    getchar();
    printf("Enter name to search (partial allowed): ");
    fgets(searchTerm, NAME_LEN, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';

    int found = 0;
    printf("\nProducts Found:\n");
    for (int i = 0; i < count; i++) {
        if (strstr(inventory[i].name, searchTerm) != NULL) {
            printf("| %d | %s | %d | %.2f |\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].quantity,
                   inventory[i].price);
            found = 1;
        }
    }
    if (!found)
        printf("No products match that name.\n");
}

void searchByPriceRange(Product *inventory, int count) {
    float minPrice, maxPrice;
    printf("Enter minimum price: ");
    if (scanf("%f", &minPrice) != 1) {
        printf("Invalid minimum price.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter maximum price: ");
    if (scanf("%f", &maxPrice) != 1 || maxPrice < minPrice) {
        printf("Invalid maximum price.\n");
        while (getchar() != '\n');
        return;
    }

    int found = 0;
    printf("\nProducts in price range:\n");
    for (int i = 0; i < count; i++) {
        if (inventory[i].price >= minPrice && inventory[i].price <= maxPrice) {
            printf("| %d | %s | %d | %.2f |\n",
                   inventory[i].id,
                   inventory[i].name,
                   inventory[i].quantity,
                   inventory[i].price);
            found = 1;
        }
    }
    if (!found)
        printf("No products in this price range.\n");
}

void deleteProduct(Product **inventory, int *count) {
    if (*count == 0) {
        printf("No products to delete.\n");
        return;
    }

    int targetId;
    printf("Enter Product ID to delete: ");
    if (scanf("%d", &targetId) != 1) {
        printf("Invalid Product ID.\n");
        while (getchar() != '\n');
        return;
    }

    int targetIndex = -1;
    for (int i = 0; i < *count; i++) {
        if ((*inventory)[i].id == targetId) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        printf("Product ID not found.\n");
        return;
    }

    for (int i = targetIndex; i < *count - 1; i++) {
        (*inventory)[i] = (*inventory)[i + 1];
    }

    *inventory = (Product *)realloc(*inventory, (*count - 1) * sizeof(Product));
    (*count)--;

    printf("Product deleted successfully!\n");
}

int main() {
    int totalProducts;
    Product *inventory = NULL;

    printf("Enter initial number of products: ");
    if (scanf("%d", &totalProducts) != 1 || totalProducts < 0) {
        printf("Invalid number of products.\n");
        return 1;
    }
    getchar();

    inventory = (Product *)calloc(totalProducts, sizeof(Product));
    if (!inventory) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < totalProducts; i++) {
        printf("\nEnter details for product %d:\n", i + 1);
        printf("Product ID (1–10000): ");
        if (scanf("%d", &inventory[i].id) != 1 || inventory[i].id < 1 || inventory[i].id > 10000) {
            printf("Invalid Product ID.\n");
            while (getchar() != '\n');
            i--;
            continue;
        }
        getchar();

        printf("Product Name: ");
        fgets(inventory[i].name, NAME_LEN, stdin);
        inventory[i].name[strcspn(inventory[i].name, "\n")] = '\0';
        if (strlen(inventory[i].name) == 0) {
            printf("Product name cannot be empty.\n");
            i--;
            continue;
        }

        printf("Product Price: ");
        if (scanf("%f", &inventory[i].price) != 1 || inventory[i].price <= 0) {
            printf("Invalid price.\n");
            while (getchar() != '\n');
            i--;
            continue;
        }

        printf("Product Quantity: ");
        if (scanf("%d", &inventory[i].quantity) != 1 || inventory[i].quantity < 0) {
            printf("Invalid quantity.\n");
            while (getchar() != '\n');
            i--;
            continue;
        }
    }

    int choice;
    do {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addProduct(&inventory, &totalProducts);
                break;
            case 2:
                viewProducts(inventory, totalProducts);
                break;
            case 3:
                updateQuantity(inventory, totalProducts);
                break;
            case 4:
                searchByID(inventory, totalProducts);
                break;
            case 5:
                searchByName(inventory, totalProducts);
                break;
            case 6:
                searchByPriceRange(inventory, totalProducts);
                break;
            case 7:
                deleteProduct(&inventory, &totalProducts);
                break;
            case 8:
                printf("Releasing memory and exiting program...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    free(inventory);
    return 0;
}
