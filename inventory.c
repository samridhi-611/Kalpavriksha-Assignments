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
    printf("Enter details of the new product:\n");
    printf("Product ID: ");
    scanf("%d", &newProduct->id);
    getchar();

    printf("Product Name: ");
    fgets(newProduct->name, NAME_LEN, stdin);
    newProduct->name[strcspn(newProduct->name, "\n")] = '\0';

    printf("Product Price: ");
    scanf("%f", &newProduct->price);

    printf("Product Quantity: ");
    scanf("%d", &newProduct->quantity);

    (*count)++;
    printf("Product added successfully!\n");
}

void viewProducts(Product *inventory, int count) {
    if (count == 0) {
        printf("No products available.\n");
        return;
    }

    printf("========= PRODUCT LIST =========\n");
    for (int productIndex = 0; productIndex < count; productIndex++) {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               inventory[productIndex].id,
               inventory[productIndex].name,
               inventory[productIndex].price,
               inventory[productIndex].quantity);
    }
}

void updateQuantity(Product *inventory, int count) {
    int targetId, updatedQuantity;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &targetId);

    for (int productIndex = 0; productIndex < count; productIndex++) {
        if (inventory[productIndex].id == targetId) {
            printf("Enter new Quantity: ");
            scanf("%d", &updatedQuantity);
            inventory[productIndex].quantity = updatedQuantity;
            printf("Quantity updated successfully!\n");
            return;
        }
    }
    printf("Product ID not found.\n");
}

void searchByID(Product *inventory, int count) {
    int targetId;
    printf("Enter Product ID to search: ");
    scanf("%d", &targetId);

    for (int productIndex = 0; productIndex < count; productIndex++) {
        if (inventory[productIndex].id == targetId) {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[productIndex].id,
                   inventory[productIndex].name,
                   inventory[productIndex].price,
                   inventory[productIndex].quantity);
            return;
        }
    }
    printf("Product not found.\n");
}

void searchByName(Product *inventory, int count) {
    char searchTerm[NAME_LEN];
    printf("Enter name to search (partial allowed): ");
    fgets(searchTerm, NAME_LEN, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';

    int found = 0;
    printf("Products Found:\n");
    for (int productIndex = 0; productIndex < count; productIndex++) {
        if (strstr(inventory[productIndex].name, searchTerm) != NULL) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[productIndex].id,
                   inventory[productIndex].name,
                   inventory[productIndex].price,
                   inventory[productIndex].quantity);
            found = 1;
        }
    }
    if (!found)
        printf("No products match that name.\n");
}

void searchByPriceRange(Product *inventory, int count) {
    float minPrice, maxPrice;
    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);

    int found = 0;
    printf("Products in price range:\n");
    for (int productIndex = 0; productIndex < count; productIndex++) {
        if (inventory[productIndex].price >= minPrice && inventory[productIndex].price <= maxPrice) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[productIndex].id,
                   inventory[productIndex].name,
                   inventory[productIndex].price,
                   inventory[productIndex].quantity);
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
    scanf("%d", &targetId);

    int targetIndex = -1;
    for (int productIndex = 0; productIndex < *count; productIndex++) {
        if ((*inventory)[productIndex].id == targetId) {
            targetIndex = productIndex;
            break;
        }
    }

    if (targetIndex == -1) {
        printf("Product ID not found.\n");
        return;
    }

    for (int moveIndex = targetIndex; moveIndex < *count - 1; moveIndex++) {
        (*inventory)[moveIndex] = (*inventory)[moveIndex + 1];
    }

    *inventory = (Product *)realloc(*inventory, (*count - 1) * sizeof(Product));
    (*count)--;

    printf("Product deleted successfully!\n");
}

int main() {
    int totalProducts;
    Product *inventory = NULL;

    printf("Enter initial number of products: ");
    scanf("%d", &totalProducts);
    getchar();

    inventory = (Product *)calloc(totalProducts, sizeof(Product));
    if (!inventory) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        printf("\nEnter details for product %d:\n", productIndex + 1);
        printf("Product ID: ");
        scanf("%d", &inventory[productIndex].id);
        getchar();

        printf("Product Name: ");
        fgets(inventory[productIndex].name, NAME_LEN, stdin);
        inventory[productIndex].name[strcspn(inventory[productIndex].name, "\n")] = '\0';

        printf("Product Price: ");
        scanf("%f", &inventory[productIndex].price);

        printf("Product Quantity: ");
        scanf("%d", &inventory[productIndex].quantity);
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

