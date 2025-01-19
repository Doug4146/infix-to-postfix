#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct charStack {
    char *array;
    int top;
} charStack;

typedef struct floatStack {
    float *array;
    int top;
} floatStack;

typedef enum {
    SUBTRACT, 
    ADD, 
    MULTIPLY, 
    DIVIDE, 
    LEFTPAREN, 
    RIGHTPAREN, 
    OPERAND} operators;

charStack* init_charStack();
void push_charStack(charStack* stack, char character);
char pop_charStack(charStack* stack);
int isCharStackEmpty(charStack* stack);

floatStack* init_floatStack();
void push_floatStack(floatStack* stack, float num);
float pop_floatStack(floatStack* stack);
int isFloatStackEmpty(floatStack* stack);

operators charToOperator(char value);
char* infix_to_postfix(char* expression);
float postfix_to_float(char* expression);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: ./infix_converter \"expression1\" \"expression2\" ...\n");
        return 1;
    }

    char* postFix[argc - 1];
    for (int i = 0; i < argc - 1; i++) {
        postFix[i] = NULL;
    }

    for (int j = 0; j < argc - 1; j++) {
        postFix[j] = infix_to_postfix(argv[j + 1]);
        printf("\nInfix expression #%d: %s\n", j + 1, argv[j + 1]);
        printf("Postfix expression: %s\n", postFix[j]);
        printf("This evaluates to: %.2f\n", postfix_to_float(postFix[j]));
        free(postFix[j]);
    }
    
    return 0;
}

charStack* init_charStack() {
    charStack* newStack = (charStack *)malloc(sizeof(charStack));
    if (newStack == NULL) {
        return NULL;
    }
    newStack->array = (char *)malloc(20 * sizeof(char));
    if (newStack->array == NULL) {
        free(newStack);
        return NULL;
    }
    newStack->top = -1;
    return newStack;
}

void push_charStack(charStack* stack, char character) {
    stack->array[++stack->top] = character; 
}

char pop_charStack(charStack* stack) {
    return stack->array[stack->top--];
}

int isCharStackEmpty(charStack* stack) {
    return (stack->top == -1);
}

floatStack* init_floatStack() {
    floatStack* newStack = (floatStack *)malloc(sizeof(floatStack));
    if (newStack == NULL) {
        return NULL;
    }
    newStack->array = (float *)malloc(20 * sizeof(float));
    if (newStack->array == NULL) {
        free(newStack);
        return NULL;
    }
    newStack->top = -1;
    return newStack;
}

void push_floatStack(floatStack* stack, float num) {
    stack->array[++stack->top] = num; 
}

float pop_floatStack(floatStack* stack) {
    return stack->array[stack->top--];
}

int isFloatStackEmpty(floatStack* stack) {
    return (stack->top == -1);
}

operators charToOperator(char value) {
    switch (value) {
        case '-': return SUBTRACT;
        case '+': return ADD;
        case '*': return MULTIPLY;
        case '/': return DIVIDE;
        case '(': return LEFTPAREN;
        case ')': return RIGHTPAREN;
        default: return OPERAND;
    }
}

char* infix_to_postfix(char* expression) {
    int size = strlen(expression);
    charStack* postFix = init_charStack();
    charStack* stack = init_charStack();

    for (int i = 0; i < size; i++) {
        operators expressionOperator = charToOperator(expression[i]);
        operators stackOperator = isCharStackEmpty(stack) ? OPERAND : charToOperator(stack->array[stack->top]);

        if (expressionOperator == OPERAND) {
            push_charStack(postFix, expression[i]);
        } else if (expressionOperator == LEFTPAREN) {
            push_charStack(stack, expression[i]);
        } else if (expressionOperator == RIGHTPAREN) {
            while (!isCharStackEmpty(stack) && stack->array[stack->top] != '(') {
                push_charStack(postFix, pop_charStack(stack));
            }
            pop_charStack(stack); 
        } else {
            while (!isCharStackEmpty(stack) && stackOperator != LEFTPAREN &&
                   expressionOperator <= stackOperator) {
                push_charStack(postFix, pop_charStack(stack));
                stackOperator = isCharStackEmpty(stack) ? OPERAND : charToOperator(stack->array[stack->top]);
            }
            push_charStack(stack, expression[i]);
        }
    }

    while (!isCharStackEmpty(stack)) {
        push_charStack(postFix, pop_charStack(stack));
    }

    char* postFixString = (char*)malloc((postFix->top + 2) * sizeof(char));
    for (int i = 0; i <= postFix->top; i++) {
        postFixString[i] = postFix->array[i];
    }
    postFixString[postFix->top + 1] = '\0';

    free(stack->array);
    free(stack);
    free(postFix->array);
    free(postFix);

    return postFixString;
}

float postfix_to_float(char* expression) {
    int size = strlen(expression);
    floatStack* stack = init_floatStack();

    for (int i = 0; i < size; i++) {
        operators currentElement = charToOperator(expression[i]);

        if (currentElement == OPERAND) {
            float number = expression[i] - '0';
            push_floatStack(stack, number);
        } else {
            float a = pop_floatStack(stack);
            float b = pop_floatStack(stack);
            float result;

            switch (expression[i]) {
                case '-': result = b - a; break;
                case '+': result = b + a; break;
                case '*': result = b * a; break;
                case '/': result = b / a; break;
                default: result = 0;
            }
            push_floatStack(stack, result);
        }
    }

    float answer = pop_floatStack(stack);
    free(stack->array);
    free(stack);

    return answer;
}
