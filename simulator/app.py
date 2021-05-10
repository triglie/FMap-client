from datetime import datetime
import threading
import time
import random

province_coords = {
    'Palermo':      '(38,1121; 13,3366)', 
    'Catania':      '(37,5013; 15,0742)', 
    'Messina':      '(38,1943; 15,5505)', 
    'Agrigento':    '(37,3219; 13,5896)', 
    'Enna':         '(37,5599; 14,2900)', 
    'Trapani':      '(38,0171; 12,5453)', 
    'Caltanissetta':'(37,4888; 14,0458)', 
    'Siracusa':     '(37,0862; 15,2738)', 
    'Ragusa':       '(36,9293; 14,7180)'
} 

def generate_path_by_province(province, dir_path='../logs', date_format='%d-%m-%Y'):
    """ Generate a log filename appending the current date to a province name """
    current_date = datetime.now().strftime(date_format)
    return f'{dir_path}/{province}-{current_date}.log'

def generate_random_PI_by_province(province):
    """
    Dobbiamo far si che data una provincia si generino PI all'interno di 
    un sottoinsieme fisso. Questo serve poiché dobbiamo simulare che in una
    certa provincia prendano bene solo alcune stazioni radio.  
    Idea: possiamo utilizzare una funzione hash 
    """
    return '1000'

def microcontroller(province): 
    logpath = generate_path_by_province(province)
    # questo va cambiato in un while(true)
    # o va utilizzato un determinato criterio di terminazione 
    #for _ in range(15):
    while(True):
        with open(logpath, 'a') as log:
            data  = f'province={province} '
            data += f'coords={province_coords.get(province)} '
            data += f'PI={generate_random_PI_by_province(province)} '
            #https://www.speedcheck.org/it/wiki/rssi/
            data += f'RSSI={random.randint(-120,0)} \n'            
            log.write(data)
            time.sleep(random.randint(1,3))

if __name__ == "__main__":
    print("Starting threads")
    for province in province_coords:
        threading.Thread(target=microcontroller, args=(province, )).start()