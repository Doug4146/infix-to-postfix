#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct charStack {
    char *array;
    int top;
} charStack;

typedef struct intStack {
    int *array;
    int top;
} intStack;

enum operators {SUBTRACT, ADD, MULTIPLY, DIVIDE, LEFTPAREN, RIGHTPAREN, OPERAND};

charStack* init_charStack();
void push_charStack(charStack* stack, char character);
char pop_charStack(charStack* stack);
int isCharStackEmpty(charStack* stack);

intStack* init_intStack();
void push_intStack(intStack* stack, int num);
int pop_intStack(intStack* stack);
int isIntStackEmpty(intStack* stack);

operators charToOperator(char value);
char* infix_to_postfix(char* expression);
int postfix_to_int(char* expression);



int main(int argc, char *argv[]) {

    // argc-1 = #infix strings input
    // argv -> array containing strings

    char* postFix[argc-1];
    for (int i = 0; i < argc-1; i++) {
        postFix[i] = NULL;
    }

    for (int j = 0; j < argc-1; j++) {
        postFix[j] = infix_to_postfix(argv[j+1]);
        printf("\nInfix expression #%d: %s\n", j+1, argv[j+1]);
        printf("Postfix expression: %s\n", postFix[j]);
        printf("This evaluates to: %d\n", postfix_to_int(postFix[j]));
    }
    
    return 0;

} 

charStack* init_charStack() {
    charStack* newStack = (charStack *)malloc(sizeof(charStack));
    if (newStack == NULL) {
        return newStack;
    }
    newStack->array = (char *)malloc(20*sizeof(char));
    if (newStack->array == NULL) {
        return NULL;
    }
    newStack->top = -1;
    return newStack;
}

void push_charStack(charStack* stack, char character) {
    if (stack == NULL) {
        stack = init_charStack();
    }
    stack->array[++stack->top] = character; 
}

char pop_charStack(charStack* stack) {
    if (stack->top == -1) {
        return '0';
    }
    char pop = stack->array[stack->top];
    stack->top--;
    return pop;
}

int isCharStackEmpty(charStack* stack) {
    return (stack->top == -1);
}

intStack* init_intStack() {
    intStack* newStack = (intStack *)malloc(sizeof(intStack));
    if (newStack == NULL) {
        return newStack;
    }
    newStack->array = (int *)malloc(20*sizeof(int));
    if (newStack->array == NULL) {
        return NULL;
    }
    newStack->top = -1;
    return newStack;
}

void push_intStack(intStack* stack, int num) {
    if (stack == NULL) {
        stack = init_intStack();
    }
    stack->array[++stack->top] = num; 
}

int pop_intStack(intStack* stack) {
    if (stack->top == -1) {
        return '0';
    }
    int pop = stack->array[stack->top];
    stack->top--;
    return pop;
}

int isIntStackEmpty(intStack* stack) {
    return (stack->top == -1);
}

operators charToOperator(char value) {
    operators Operator;
    switch (value) {
        case ('-'):
            Operator = SUBTRACT;
            break;
        case ('+'):
            Operator = ADD;
            break;
        case ('*'):
            Operator = MULTIPLY;
            break;
        case ('/'):
            Operator = DIVIDE;
            break;
        case ('('):
            Operator = LEFTPAREN;
            break;
        case (')'):
            Operator = RIGHTPAREN;
            break;
        default:
            Operator = OPERAND;
    }
    return Operator;
}


char* infix_to_postfix(char* expression) {
    int size = strlen(expression);
    charStack* postFix =  init_charStack();//assume allocation went ok
    charStack* stack = init_charStack(); //create intermediate stack

    for (int i = 0; i < size; i++) {

        //What is the operator of the current element in expression
        operators expressionOperator = charToOperator(expression[i]);

        //what is the operator of the element in top of
        operators stackOperator; 
        int isEmpty = isCharStackEmpty(stack);
        if (!isEmpty) {
            stackOperator = charToOperator(stack->array[stack->top]);
        }

        //What do now now that expression element and stack element are known
        //if expression element is Operand, push onto postfix
        if (expressionOperator == OPERAND) {
            push_charStack(postFix, expression[i]);
        }
        //if expression element is leftparen, push onto postfix
        else if (expressionOperator == LEFTPAREN) {  
            push_charStack(stack, expression[i]);
        }
        //if operator is right paren, repeat pop off stack/push onto postfix till '('
        else if (expressionOperator == RIGHTPAREN) { 
            while (!isCharStackEmpty(stack) && stack->array[stack->top] != '(') {
                push_charStack(postFix, pop_charStack(stack));
            }
            pop_charStack(stack);
        } 
        //if stack is empty, or stack element is left paren or the stack is NOT emepty and expression element > stack elemenet
        //push expression element onto stack
        else if (isEmpty || (stackOperator == LEFTPAREN) || (!isEmpty && (expressionOperator > stackOperator))) { 
            push_charStack(stack, expression[i]);
        }
        else if (expressionOperator <= stackOperator) {
            operators tempStackOperator = stackOperator;
            while ((expressionOperator <= tempStackOperator) && !isCharStackEmpty(stack)) {
                tempStackOperator = charToOperator(stack->array[stack->top]);
                push_charStack(postFix, pop_charStack(stack));
            }
            push_charStack(stack, expression[i]);
        }

    }

    while (!isCharStackEmpty(stack)) {
        push_charStack(postFix, pop_charStack(stack));
    }

    //convert the postFix stack to a string
    char* postFixString = (char*)malloc((postFix->top+2)*sizeof(char));
    for (int i = 0; i <= postFix->top; i++) {
        postFixString[i] = postFix->array[i];
    }
    postFixString[postFix->top + 1] = '\0'; // Null terminate
    
    
    free(stack->array);
    free(stack);
    free(postFix->array);
    free(postFix);

    return postFixString;

}

int postfix_to_int(char* expression) {
    int expressionSize = strlen(expression);
    intStack* stack = init_intStack();
    for (int i = 0; i < expressionSize; i++) {
        operators currentElement = charToOperator(expression[i]);
        if (currentElement == OPERAND) {
            int number = expression[i] - '0';
            push_intStack(stack, number);
        }
        //assumes postFix is correct
        else {
            int a = pop_intStack(stack);
            int b = pop_intStack(stack);
            int temp; 
            switch (expression[i]) {
                case '-':
                    temp = b - a;
                    break;
                case '+':
                    temp = b + a;
                    break;
                case '*':
                    temp = b * a;
                    break;
                case '/':
                    temp = (int) b / a;
                    break;
                default:
                    break;
            }
            push_intStack(stack, temp);
        }
    }

    int answer = pop_intStack(stack);
    free(stack->array);
    free(stack);

    return answer;
}
