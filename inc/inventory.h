#pragma once
#include <iostream>
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <string>

template <typename T>

class inventory{
        private:    
            std::vector<T> items;
        public:
            void add(const T& item){
                items.push_back(item);
            }
            std::size_t size() const { 
                return items.size(); 
            }
            const T& get(std::size_t index) const {
                if (index >= items.size()) {
                    throw std::out_of_range("No item at position " + std::to_string(index));
                }
                return items[index];
            }
            void remove(std::size_t index){
                if (index >= items.size()) {
                    throw std::out_of_range("No item at position " + std::to_string(index));
                }
                items.erase(items.begin()+index);
            }
            template <typename Pred>
            std::vector<T> filter(Pred p) const {
                std::vector<T> result;
                for (const T& item : items){
                    if(p(item)){
                        result.push_back(item);
                    }
                }
                return result;
            };

            
};

struct Product{
                std::string name;
                double price;
        };