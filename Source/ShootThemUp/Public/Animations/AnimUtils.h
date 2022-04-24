#pragma once

class AnimUtils //класс содержит лишь один метод
{
public: 
    //шаблонный метод, который находит нотифай по классу анимации
    template <typename T> //передаем класс нотифая
    static T* FindNotifyByClass(UAnimSequenceBase* Animation) //передаем анимацию
    {
        if (!Animation)
            return nullptr; //если анимация пустая, выходим
        const auto NotifyEvents = Animation->Notifies; //ищем все нотифаи в анимации
        for (auto NotifyEvent : NotifyEvents)
        {
            auto AnimNotify = Cast<T>(NotifyEvent.Notify); //если класс нотифая равняется нашему
            if (AnimNotify)
            {
                return AnimNotify; //возвращаем нотифай
            }
        }
        return nullptr;
    }
};