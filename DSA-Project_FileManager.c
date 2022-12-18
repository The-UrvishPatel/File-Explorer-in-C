#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Structure for Files

typedef struct file
{
    char name[100];

} file;


//Structure for Folders

typedef struct folder
{
    char name[100];

    //Structure for Folders' list

    struct folder_list
    {
        struct folder *fd;
        struct folder_list *next, *prev;

    } folder_list;

    //Structure for Files' list

    struct file_list
    {
        struct file *fl;
        struct file_list *next, *prev;

    } file_list;

    struct folder_list *fd_list;
    struct file_list *fl_list;

    struct folder_list *last_folder;
    struct file_list *last_file;

} folder;

folder* now = NULL;

//Search Folder Function

folder *search_folder(char to_find[100], folder *current, int flag,int to_open, int go_inside)
{
    folder *found = NULL;

    struct folder_list *temp = current->fd_list;

    if (temp != NULL)
    {
        do
        {

            if (strcmp(temp->fd->name, to_find) == 0)
            {
                if(flag || to_open || go_inside)
                    return found = now = temp->fd;
                else if (flag == 0)
                    return found = current;
            }

            temp = temp->next;

        } while (temp != current->fd_list);
    }

    if(go_inside)
    return found;

    temp = current->fd_list;

    if (temp != NULL)
    {
        do
        {
            found = search_folder(to_find, temp->fd, flag,to_open, go_inside);

            if (found != NULL)
                return found;

            temp = temp->next;
        } while (temp != current->fd_list);
    
    }
    return found;
}


//Search File Function

folder *search_file(char to_find[100], folder *current)
{
    folder *found = NULL;

    struct file_list *temp = current->fl_list;

    if (temp != NULL)
    {
        do
        {

            if (strcmp(temp->fl->name, to_find) == 0)
                return found = current;

            temp = temp->next;
        } while (temp != current->fl_list);
    }


    struct folder_list *list = current->fd_list;


    if (list != NULL)
    {
        do
        {
            found = search_file(to_find, list->fd);

            if (found != NULL)
                return found;

            list = list->next;

        } while (list != current->fd_list);



    }

    return found;
}


//Add a folder

int add_folder(char name[100], folder *current)
{
    folder *new_folder = (folder *)calloc(1, sizeof(folder));

    strcpy(new_folder->name, name);

    struct folder_list *new = (struct folder_list *)calloc(1, sizeof(struct folder_list));

    if (new_folder == NULL || new == NULL)
        return 0;

    if (current->fd_list == NULL)
    {
        new->fd = new_folder;
        new->next = new;
        new->prev = new;

        current->fd_list = new;
        current->last_folder = new;
    }
    else
    {
        new->fd = new_folder;
        new->next = current->fd_list;
        new->prev = current->last_folder;
        current->last_folder->next = new;
        current->fd_list->prev = new;
        current->last_folder = new;
    }

    return 1;
}


//Add a file

int add_file(char name[100], folder *current)
{
    file *new_file = (file *)calloc(1, sizeof(file));

    strcpy(new_file->name, name);

    struct file_list *new = (struct file_list *)calloc(1, sizeof(struct file_list));

    if (new_file == NULL || new == NULL)
        return 0;

    if (current->fl_list == NULL)
    {
        new->fl = new_file;
        new->next = new;
        new->prev = new;
        current->fl_list = new;
        current->last_file = new;
    }
    else
    {
        new->fl = new_file;
        new->next = current->fl_list;
        new->prev = current->last_file;
        current->last_file->next = new;
        current->fl_list->prev = new;
        current->last_file = new;
    }

    return 1;
}

//Delete Folder

int delete_folder(char to_delete[100], folder *current,int open)
{
    int flag=0;

    struct folder_list *temp = current->fd_list;

    if (temp != NULL)
    {
        do
        {

            if (strcmp(temp->fd->name, to_delete) == 0)
            {

                if(temp->prev==temp && temp->next==temp)
                current->fd_list=NULL;
                else if(temp==current->fd_list)
                current->fd_list=temp->next;
                else if(temp==current->last_folder)
                current->last_folder=temp->prev;
                

                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;                
                temp->next = temp->prev = NULL;
                // free(temp->fd);
                // free(temp->fd);

                return 1;
            }

            temp = temp->next;

        } while (temp != current->fd_list);
    }

    if(open)
    return 0;


    temp = current->fd_list;

    if(temp!=NULL)
    {
        do
        {
            flag = delete_folder(to_delete, temp->fd,open);

            if (flag != 0)
                return flag;

            temp = temp->next;

        } while (temp != current->fd_list);

    }

    return flag;
}


//Delete File

int delete_file(char to_delete[100], folder *current,int open)
{
    int flag = 0;

    struct file_list *temp = current->fl_list;

    if (temp != NULL)
    {

        do
        {

            if (strcmp(temp->fl->name, to_delete) == 0)
            {
                if(temp->prev==temp && temp->next==temp)
                current->fl_list=NULL;
                else if(temp==current->fl_list)
                current->fl_list=temp->next;
                else if(temp==current->last_file)
                current->last_file=temp->prev;
                

                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                temp->next = temp->prev = NULL;
                // free(temp->fl);

                return 1;
            }

            temp = temp->next;

        } while (temp != current->fl_list);
    }

    if(open)
    return 0;


    struct folder_list *list = current->fd_list;


    if(list!=NULL)
    {
        do
        {
            flag = delete_file(to_delete, list->fd,open);

            if (flag != 0)
                return flag;

            list = list->next;

        } while (list != current->fd_list);
    }


    return flag;
}


//Display Folder

int show_folder(folder *current)
{
    struct folder_list *showfolder = current->fd_list;

    if (showfolder == NULL)
        return 0;

    do
    {
        printf("> %s\n", showfolder->fd->name);
        showfolder = showfolder->next;

    } while (showfolder != current->fd_list);

    return 1;
}


//Display File

int show_file(folder *current)
{
    struct file_list *showfile = current->fl_list;

    if (showfile == NULL)
        return 0;

    do
    {
        printf("> %s\n", showfile->fl->name);
        showfile = showfile->next;
    } while (showfile != current->fl_list);

    return 1;
}

int copy_folder(folder *copy, folder *current)
{
    struct folder_list *new = (struct folder_list *)calloc(1, sizeof(struct folder_list));

    if (new == NULL)
        return 0;

    if (current->fd_list == NULL)
    {
        new->fd = copy;
        new->next = new;
        new->prev = new;
        current->fd_list = new;
        current->last_folder = new;
    }
    else
    {
        new->fd = copy;
        new->next = current->fd_list;
        new->prev = current->last_folder;
        current->last_folder->next = new;
        current->fd_list->prev = new;
        current->last_folder = new;
    }

    return 1;
}



int main()
{
    folder *Main = (folder *)calloc(1, sizeof(folder));
    strcpy(Main->name, "Main");

    printf("\n\n\n### File Manager [File Management System] ###\n\n\n");


next:

    printf("\n\n 1: Add a Folder in Main Drive\n 2: Add a file to Main Drive\n 3: Add a folder to perticular folder\n 4: Add a file to perticular folder\n 5: Search Folder and show\n 6: Search File\n 7: Delete Folder\n 8: Delete File\n 9: Copy\\Paste Folder\n10: Copy\\Paste File\n11: Cut Folder\n12: Cut file\n13: Add folder in currently open folder\n14: Add file in currently open folder\n15: Delete folder in currently open folder\n16: Delete file in currently open folder\n17: Show currently open folder\n18: Go inside of any folder of corrently open folder\n0: Exit\n\n\n");

    int choice;
    printf("=> Enter the choice : ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 0:
    {
        printf("\n\n=> Exit\n\n");
        return 0;
    }
    case 1:
    {
        char name[100];
        printf("\n\n=> Enter the name of folder : ");
        scanf("%s", name);
        int check=add_folder(name, Main);   
        if(check)
        printf("\n\n=> Added Successfully!!!");
        else
        printf("\n\n=> Process Failed!!!");
        
        break;
    }
    case 2:
    {
        char name[100];
        printf("\n\n=> Enter the name of file : ");
        scanf("%s", name);
        
        int check=add_file(name, Main);
        
        if(check)
        printf("\n\n=> Added Successfully!!!");
        else
        printf("\n\n=> Process Failed!!!");

        break;
    }
    case 3:
    {
        char name[100];
        printf("\n\n=> Enter the name of folder : ");
        scanf("%s", name);

        char insideof[100];
        printf("\n\n=> Enter the name of the folder in which you want to insert this folder : ");
        scanf("%s", insideof);
        folder *found = search_folder(insideof, Main, 1, 0, 0);

        if (found == NULL)
            printf("\n\n=> Folder Doesn't Exits!!!");
        else
        {
            int check = add_folder(name, found);

            if(check)
            printf("\n\n=> Added Successfully!!!");
            else
            printf("\n\n=> Process Failed!!!");
        }

        break;
    }
    case 4:
    {
        char name[100];
        printf("\n\n=> Enter the name of file : ");
        scanf("%s", name);

        char insideof[100];
        printf("\n\n=> Enter the name of the folder in which you want to insert this file : ");
        scanf("%s", insideof);
        folder *found = search_folder(insideof, Main, 1, 0, 0);

        if (found == NULL)
            printf("\n\n=> Folder Doesn't Exits!!!");
        else
        {
            int check = add_file(name, found);

            if(check)
            printf("\n\n=> Added Successfully!!!");
            else
            printf("\n\n=> Process Failed!!!");
        }

        break;
    }
    case 5:
    {
        char name[100];
        printf("\n\n=> Enter the name of folder : ");
        scanf("%s", &name);

        folder *found;

        if(strcmp("Main",name)==0)
        found=now=Main;        
        else
        found = search_folder(name, Main, 1, 1, 0);

        if (found == NULL)
            printf("\n\n=> Folder Doesn't Exits!!!");
        else
        {
            printf("\n\n=> Folder Found!!!");

            printf("\n\n=> Showing Information contained by %s folder :\n\n",name);
            printf("\n\n=> Folder : \n\n");
            int check=show_folder(found);
            if(!check)
            printf("\n=> Empty!!!\n\n");
            printf("\n=> File : \n\n");
            check=show_file(found);
            if(!check)
            printf("\n=> Empty!!!\n\n");
        }

        break;
    }
    case 6:
    {
        char name[100];
        printf("\n\n=> Enter the name of file : ");
        scanf("%s", &name);
        
        folder *found = search_file(name, Main);

        if (found == NULL)
            printf("\n\n=> File Doesn't Exits!!!");
        else
        {
            printf("\n\n=> File Found!!!");

            printf("\n\n=> File is contained by %s folder : ",found->name);
            printf("\n\n=> Folder : \n\n");
            int check=show_folder(found);
            if(!check)
            printf("\n=> Empty!!!\n\n");
            printf("\n=> File : \n\n");
            check=show_file(found);
            if(!check)
            printf("\n=> Empty!!!\n\n");
        }

        break;
    }
    case 7:
    {
        char name[100];
        printf("\n\n=> Enter the name of the folder to be deleted : ");
        scanf("%s", &name);
        
        int check = delete_folder(name, Main,0);

        if (check)
        {
            printf("\n\n=> Folder deleted!!!");
        }
        else
        {
            printf("\n\n=> Deletion Failed or Folder doesn't exist!!!");
        }

        break;
    }
    case 8:
    {
        char name[100];
        printf("\n\n=> Enter the name of the file to be deleted : ");
        scanf("%s", &name);
        int check = delete_file(name, Main,0);

        if (check)
        {
            printf("\n\n=> File deleted!!!");
        }
        else
        {
            printf("\n\n=> Deletion Failed or File doesn't exist!!!");
        }

        break;
    }
    case 9:
    {
        char name[100];
        
        printf("\n\n=> Enter the name of the folder to be copied:");
        scanf("%s",&name);
        folder * copy = search_folder(name, Main,1, 0, 0);
        
        char insideof[100];
        printf("\n\n=> Enter the name of the folder in which you want to copy this folder : ");
        scanf("%s", insideof);

        if (copy!=NULL)
        {
            
            if(strcmp(insideof,"Main") == 0)
            {
                int check=copy_folder(copy, Main);
                
                if(check)
                    printf("\n\n=> Copied Successfully!!!");
                else
                    printf("\n\n=> Process Failed!!!");
            }
            else
            {
                folder *found = search_folder(insideof, Main, 1, 0, 0);
                
                if (found == NULL)
                    printf("\n\n=> Folder inside which you want to copy Doesn't Exits You can't copy it!!!");
                else
                {
                    int check = copy_folder(copy, found);
                    
                    if(check)
                    printf("\n\n=> Copied Successfully!!!");
                    else
                    printf("\n\n=> Process Failed!!!");
                }
            }
        }
        else
        {
            printf("\n\n=> Folder that you want to copy not found!!!");
        }
        break;
    }
    case 10:
    {
        char name[100];

        printf("\n\n=> Enter the name of the file to be copied:");
        scanf("%s",&name);
        folder * copy = search_file(name, Main);
        
        char insideof[100];
        printf("\n\n=> Enter the name of the folder in which you want to copy this folder : ");
        scanf("%s", insideof);

        if (copy!=NULL)
        {
            
            if(strcmp(insideof,"Main") == 0)
            {
                int check=add_file(name, Main);
                
                if(check)
                    printf("\n\n=> Copied Successfully!!!");
                else
                    printf("\n\n=> Process Failed!!!");
            }
            else
            {
                folder *found = search_folder(insideof, Main, 1, 0, 0);
                
                if (found == NULL)
                    printf("\n\n=> Folder inside which you want to copy Doesn't Exits You can't copy it!!!");
                else
                {
                    int check = add_file(name, found);
                    
                    if(check)
                    printf("\n\n=> Copied Successfully!!!");
                    else
                    printf("\n\n=> Process Failed!!!");
                }
            }
        }
        else
        {
            printf("\n\n=> File that you want to copy not found!!!");
        }
        break;
    }
    case 11:
    {
        char name[100];
        
        printf("\n\n=> Enter the name of the folder to be cut: ");
        scanf("%s",&name);
        folder *cut = search_folder(name, Main, 1, 0, 0);

        char insideof[100];
        printf("\n\n=> Enter the name of the folder in which you want to cut this folder : ");
        scanf("%s", insideof);

        if (cut!=NULL)
        {    
            if(strcmp(insideof,"Main") == 0)
            {
                int check=copy_folder(cut, Main);
                int delete = delete_folder(name,Main,0);
                
                if(check && delete)
                printf("\n\n=> Cut Successfully!!!");
                else
                printf("\n\n=> Process Failed!!!");
            }
            else
            {
                folder *found = search_folder(insideof, Main, 1, 0,0);
            
                if (found == NULL)
                printf("\n\n=> Folder you want to copy in is Doesn't Exits You can't cut it!!!");
                else
                {
                    int check = copy_folder(cut, found);
                    int delete = delete_folder(name,Main,0);
                    
                    if(check && delete)
                    printf("\n\n=> Copied Successfully!!!");
                    else
                    printf("\n\n=> Process Failed!!!");
                }
            }
        }
        else
        {
            printf("\n\n=> Folder that you want to cut not found!!!");
        }

        break;
    }
    case 12:
    {
        char name[100];
        
        printf("\n\n=> Enter the name of the file to be cut:");
        scanf("%s",&name);
        folder * cut = search_file(name, Main);
        
        char insideof[100];
        printf("\n\n=> Enter the name of the folder in which you want to cut this folder : ");
        scanf("%s", insideof);
            
        if (cut!=NULL)
        {
            if(strcmp(insideof,"Main") == 0)
            {
                int check=add_file(name, Main);
                int delete = delete_file(name,Main,0);
            
                if(check && delete)
                printf("\n\n=> Cut Successfully!!!");
                else
                printf("\n\n=> Process Failed!!!");
            }
            else
            {
                folder *found = search_folder(insideof, Main, 1, 0,0);
                
                if (found == NULL)
                printf("\n\n=> Folder you want to copy in Doesn't Exits You can't cut it!!!");
                else
                {
                    int check = add_file(name, found);
                    int delete = delete_file(name,Main,0);
                    
                    if(check && delete)
                    printf("\n\n=> Copied Successfully!!!");
                    else
                    printf("\n\n=> Process Failed!!!");
                }
            }
        }
        else
        {
            printf("\n\n=> File that you want to cut not found, you cann't cut the file!!!\n");
        }

        break;
    }
    case 13:
    {
        if(now==NULL)
        {
            printf("\n\n=> First, Open a folder by searching it!!!");
            goto next;
        }

        char name[100];
        printf("\n\n=> Enter the name of folder : ");
        scanf("%s", name);
        
        int check=add_folder(name, now);   
        if(check)
        printf("\n\n=> Added Successfully!!!");
        else
        printf("\n\n=> Process Failed!!!");
        
        break;
    }
    case 14:
    {
        if(now==NULL)
        {
            printf("\n\n=> First, Open a folder by searching it!!!");
            goto next;
        }

        char name[100];
        printf("\n\n=> Enter the name of file : ");
        scanf("%s", name);
        
        int check=add_file(name, now);
        
        if(check)
        printf("\n\n=> Added Successfully!!!");
        else
        printf("\n\n=> Process Failed!!!");

        break;
    }
    case 15:
    {
        if(now==NULL)
        {
            printf("\n\n=> First, Open a folder by searching it!!!");
            goto next;
        }

        char name[100];
        printf("\n\n=> Enter the name of the folder to be deleted : ");
        scanf("%s", &name);
        
        int check = delete_folder(name, now,1);

        if (check)
        {
            printf("\n\n=> Folder deleted!!!");
        }
        else
        {
            printf("\n\n=> Deletion Failed or Folder doesn't exist!!!");
        }

        break;        
    }
    case 16:
    {
        if(now==NULL)
        {
            printf("\n\n=> First, Open a folder by searching it!!!");
            goto next;
        }

        char name[100];
        printf("\n\n=> Enter the name of the file to be deleted : ");
        scanf("%s", &name);
        
        int check = delete_file(name, now,1);

        if (check)
        {
            printf("\n\n=> File deleted!!!");
        }
        else
        {
            printf("\n\n=> Deletion Failed or File doesn't exist!!!");
        }

        break;
    }
    case 17:
    {
        if(now==NULL)
        {
            printf("\n\n=> First, Open a folder by searching it!!!");
            goto next;
        }


        printf("\n\n=> Showing Information contained by %s folder :\n\n",now->name);
        
        printf("\n\n=> Folder : \n\n");
        int check=show_folder(now);
        if(!check)
        printf("\n=> Empty!!!\n\n");
        
        printf("\n\n=> File : \n\n");
        check=show_file(now);
        if(!check)
        printf("\n=> Empty!!!\n\n");

        break;
    }
    case 18:
    {
        if(now==NULL)
        {
            printf("\n\n=> First, Open a folder by searching it!!!");
            goto next;
        }

        char name[100];
        printf("\n\n=> Enter the name of a folder you want to open of this folder : ");
        scanf("%s",&name);

        folder* check = search_folder(name, now, 0, 0, 1);

        if(check!=NULL)
        printf("\n\n=> You are now inside of %s folder.",name);
        else
        printf("\n\n=> Folder not found!!!");


        break;
    }
    default:
    {
        printf("\n\n=> Enter the valid choice");
        goto next;
        break;
    }
    
    }

    goto next;

    return 0;
}





















/*  Output : 



### File Manager [File Management System] ###




 1: Add a Folder in Main Drive       
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder  
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 1


=> Enter the name of folder : urvish


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 1


=> Enter the name of folder : ved


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 1


=> Enter the name of folder : utsav


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 2


=> Enter the name of file : file1


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 2


=> Enter the name of file : file2


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : Main


=> Folder Found!!!

=> Showing Information contained by Main folder :



=> Folder :

> urvish
> ved
> utsav


=> File :

> file1
> file2


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 3


=> Enter the name of folder : u1


=> Enter the name of the folder in which you want to insert this folder : urvish


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 3


=> Enter the name of folder : u2


=> Enter the name of the folder in which you want to insert this folder : urvish


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 3


=> Enter the name of folder : u3


=> Enter the name of the folder in which you want to insert this folder : urvish


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : urvish


=> Folder Found!!!

=> Showing Information contained by urvish folder :



=> Folder :

> u1
> u2
> u3


=> File :



=> Empty!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 4


=> Enter the name of file : ufile1


=> Enter the name of the folder in which you want to insert this file : urvish


=> Added Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : urvish


=> Folder Found!!!

=> Showing Information contained by urvish folder :



=> Folder :

> u1
> u2
> u3


=> File :

> ufile1


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 6


=> Enter the name of file : ufile1


=> File Found!!!

=> File is contained by urvish folder :

=> Folder :

> u1
> u2
> u3


=> File :

> ufile1


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 9


=> Enter the name of the folder to be copied:urvish


=> Enter the name of the folder in which you want to copy this folder : ved


=> Copied Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : ved


=> Folder Found!!!

=> Showing Information contained by ved folder :



=> Folder :

> urvish


=> File :



=> Empty!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 10


=> Enter the name of the file to be copied:ufile1


=> Enter the name of the folder in which you want to copy this folder : utsav


=> Copied Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : utsav


=> Folder Found!!!

=> Showing Information contained by utsav folder :



=> Folder :



=> Empty!!!

=> File :

> ufile1


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 12


=> Enter the name of the file to be cut:8     


=> Enter the name of the folder in which you want to cut this folder : ufile1


=> File that you want to cut not found, you cann't cut the file!!!


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 8


=> Enter the name of the file to be deleted : ufile1


=> File deleted!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : urvish


=> Folder Found!!!

=> Showing Information contained by urvish folder :



=> Folder :

> u1
> u2
> u3


=> File :



=> Empty!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 12


=> Enter the name of the file to be cut:u1   


=> Enter the name of the folder in which you want to cut this folder : urvish


=> File that you want to cut not found, you cann't cut the file!!!


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 12


=> Enter the name of the file to be cut:file1


=> Enter the name of the folder in which you want to cut this folder : urvish


=> Copied Successfully!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : urvish


=> Folder Found!!!

=> Showing Information contained by urvish folder :



=> Folder :

> u1
> u2
> u3


=> File :

> file1


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : Main


=> Folder Found!!!

=> Showing Information contained by Main folder :



=> Folder :

> urvish
> ved
> utsav


=> File :

> file2


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 7


=> Enter the name of the folder to be deleted : urvish


=> Folder deleted!!!

 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 5


=> Enter the name of folder : Main


=> Folder Found!!!

=> Showing Information contained by Main folder :



=> Folder :

> ved
> utsav


=> File :

> file2


 1: Add a Folder in Main Drive
 2: Add a file to Main Drive
 3: Add a folder to perticular folder
 4: Add a file to perticular folder
 5: Search Folder and show
 6: Search File
 7: Delete Folder
 8: Delete File
 9: Copy\Paste Folder
10: Copy\Paste File
11: Cut Folder
12: Cut file
 0: Exit


=> Enter the choice : 0


=> Exit




*/