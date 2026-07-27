import { useEffect, useState } from 'react';
import { api } from '../api';

// Pastille verte/rouge dans la navbar indiquant si le backend C++ répond.
// Interroge /api/stats toutes les 5 secondes.
export default function BackendStatus() {
  const [online, setOnline] = useState(null); // null = en cours de vérif

  const check = () => {
    api.getStats()
      .then(() => setOnline(true))
      .catch(() => setOnline(false));
  };

  useEffect(() => {
    check();
    const interval = setInterval(check, 5000);
    return () => clearInterval(interval);
  }, []);

  if (online === null) return <span className="status-dot checking">●</span>;

  return (
    <span className={`status-dot ${online ? 'online' : 'offline'}`} title={online ? 'Backend connecté (port 8080)' : 'Backend inaccessible — lancez cgpa_calculator.exe'}>
      ● {online ? 'Backend connecté' : 'Backend hors ligne'}
    </span>
  );
}
