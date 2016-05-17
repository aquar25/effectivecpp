#include <iostream>
#include <memory>
#include <tr1/memory>
#include "../include/baseitem.h"
#include <SDL2/SDL.h>

using namespace std;

void ShowItem(BaseItem& item)
{
    cout<<"Item id: "<<item.GetID()<<endl;
}

void ShowItem(int data)
{
    cout<<"Show integer: "<<data<<endl;
}

#define MAX_VALUE(a, b) ShowItem((a) > (b) ? (a) : (b))

template<typename T>
inline void MaxValue(const T& a, const T& b)
{
    ShowItem(a > b ? a : b);
}

void ManageObjects(int count)
{
    std::tr1::shared_ptr<BaseItem> pItem(new BaseItem(5));
    std::tr1::shared_ptr<BaseItem> pItem2(pItem); // pItem2指向对象，pItem也指向对象，引用计数为2
    pItem = pItem2;  // pItem2指向对象，pItem也指向对象
    pItem2->Print();
    pItem->Print();
    //....
    if(0 == count)
    {
        return ;
    }
}

namespace {
// namespace中的内容不缩进
enum ALGIN { RIGHT, LEFT, MIDLE};
ALGIN algin() { return RIGHT;};

} // namespace

/******************************************************************************
	example00: Setting up SDL
******************************************************************************/
void example00()
{
    SDL_Window	*pWindow = NULL;
    SDL_Renderer*pRenderer = NULL;

    // 1. initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf ("SDL initialize fail:%s\n", SDL_GetError());
        return;
    }

    // 2. create window
    pWindow = SDL_CreateWindow("example00:Setting up SDL",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               640, 480,
                               SDL_WINDOW_SHOWN);
    if (NULL == pWindow)
    {
        printf ("Create window fail:%s\n", SDL_GetError());
    }

    // 3. create renderer
    pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

    // 4. clear the window to green
    SDL_SetRenderDrawColor(pRenderer,0,255,0,255);
    SDL_RenderClear(pRenderer);

    // 5. show the window
    SDL_RenderPresent(pRenderer);

    SDL_Delay(5000);  // for display

    // 6. exit
    SDL_Quit();
}

int main()
{

    char szInfo[50] = {0};
    float fValue = 6.625f;
    int nPresion = 2;
    //fValue += 5/pow(10,nPresion+1);
    sprintf(szInfo, "%.*f", nPresion, fValue);
    cout << szInfo << endl;

    //ShowItem(34);
    Game* cod = new Game();
    Game* gta = new Game();
    gta->SetName("gta");
    cod->SetName("Call of Duty");
    Game* sim = new Game(*cod);
    ManageObjects(0);

    Container container;
    container.addItem(cod);
    container.addItem(gta);
    container.addItem(sim);
    container.doTransform();

    //getchar();
    //example00();

    return 0;
}
