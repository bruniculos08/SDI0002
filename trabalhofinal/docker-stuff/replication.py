from cassandra.cluster import Cluster
import time
import random
import numpy as np
from matplotlib import pyplot as plt

cluster = Cluster()
cluster = Cluster(['172.17.0.2','172.17.0.3','172.17.0.4'], port=9042)

session = cluster.connect('sensores') 
session.set_keyspace('sensores')

session.execute('USE sensores')

# session.execute('TRUNCATE temperaturas')

# query = "INSERT INTO temperaturas(id_sensor, momento_sensor, temperatura_sensor) VALUES ('01', to_timestamp(now()), 25)"
# session.execute(query)

# rows = session.execute('SELECT * FROM sensores.temperaturas')
# for i in rows: 
#     print(i)

n = session.execute('SELECT COUNT(*) FROM sensores.temperaturas')
print("Número de linhas na tabela 'sensores.temperaturas': " + n)

# session.execute('TRUNCATE temperaturas')
