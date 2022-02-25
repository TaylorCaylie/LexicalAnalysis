        if(strcmp(tokens[i],"BEGIN") == 0){
            if(strcmp(tokens[i-1],'::==') != 0){
                printf("assignment lexical item not found before begin\n");
                return 0;
            }

            printf("Sequence Begin Reached");
            started = 1;
        }