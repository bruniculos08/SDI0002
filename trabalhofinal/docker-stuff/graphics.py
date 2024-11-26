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
rows = session.execute('SELECT * FROM sensores.temperaturas')

for i in rows: 
    print(i)

def deleteAllCassandra():
    exclude = session.execute('SELECT id_sensor FROM sensores.temperaturas WHERE temperatura_graus = 25.0 ALLOW FILTERING')
    for id in exclude:
        session.execute(f"DELETE FROM sensores.temperaturas WHERE id_sensor = '{id[0]}'")

session.execute('TRUNCATE temperaturas')

rows_per_round = []
insert_times = []
select_times = []
max_exp = 20
for i in range(4, max_exp + 1, 4):
    # i = min(18, i)
    rows_num = 2 ** i
    print(i)
    rows_per_round.append(rows_num)

    start = time.time()
    for j in range(0, rows_num):
        query = f"INSERT INTO temperaturas(id_sensor, momento_sensor, temperatura_graus) VALUES ('{j + 1}', now(), 25)"
        session.execute(query)
        # if(j % 100 == 0): print(str(float(j/rows_num) * 100.0) + "%")
    duration = time.time() - start
    insert_times.append(duration)
    
    rand_id = random.randrange(1, rows_num, 1)
    # rand_id = int(rows_num/2)
    start = time.time()
    result = session.execute(f"SELECT id_sensor, to_timestamp(momento_sensor), temperatura_graus FROM sensores.temperaturas where id_sensor = '{rand_id}'")
    duration = time.time() - start
    print(result[0])
    select_times.append(duration)
    session.execute('TRUNCATE temperaturas')
    # deleteAllCassandra()

# session.execute('TRUNCATE temperaturas')
# session.execute('DELETE * FROM sensores.temperaturas')
print(['{0:.7f}'.format(t) for t in insert_times])
print(['{0:.7f}'.format(t) for t in select_times])
print(rows_per_round)

plt.plot(rows_per_round, insert_times, linestyle='--', marker='o', color='b', label='insert times')
plt.legend()
plt.savefig("insert_times.png")
plt.clf()

plt.plot(rows_per_round, insert_times, linestyle='--', marker='o', color='b', label='insert times')
plt.xscale('log', base=2)
plt.legend()
plt.savefig("insert_times_log.png")
plt.clf()

plt.plot(rows_per_round, select_times, linestyle='--', marker='o', color='b', label='select times')
plt.legend()
plt.savefig("select_times.png")
plt.clf()

plt.plot(rows_per_round, select_times, linestyle='--', marker='o', color='b', label='select times')
plt.xscale('log', base=2)
plt.legend()
plt.savefig("select_times_log.png")
plt.clf()