import { useEffect, useState } from 'react';
import { api } from '../api';

function StatCard({ label, value, color }) {
  return (
    <div className="stat-card" style={{ borderTop: `4px solid ${color}` }}>
      <div className="stat-value">{value}</div>
      <div className="stat-label">{label}</div>
    </div>
  );
}

function CgpaBar({ name, cgpa }) {
  const pct = (cgpa / 4.0) * 100;
  const color = cgpa >= 3.5 ? '#22c55e' : cgpa >= 2.5 ? '#f59e0b' : '#ef4444';
  return (
    <div className="cgpa-bar-row">
      <span className="cgpa-bar-name">{name}</span>
      <div className="cgpa-bar-track">
        <div className="cgpa-bar-fill" style={{ width: `${pct}%`, background: color }} />
      </div>
      <span className="cgpa-bar-value">{cgpa.toFixed(2)}</span>
    </div>
  );
}

export default function Dashboard({ onNavigate }) {
  const [stats,    setStats]    = useState(null);
  const [students, setStudents] = useState([]);
  const [error,    setError]    = useState('');

  useEffect(() => {
    api.getStats().then(r => setStats(r.data)).catch(() => setError('Backend inaccessible — démarrez cgpa_calculator.exe'));
    api.sortStudents('cgpa').then(r => setStudents(r.data)).catch(() => {});
  }, []);

  if (error) return (
    <div className="error-banner">
      ⚠️ {error}
      <div style={{ marginTop: 8, fontSize: 13, opacity: 0.8 }}>
        Lancez <code>cgpa_calculator.exe</code> puis rechargez la page.
      </div>
    </div>
  );

  return (
    <div>
      <h1 className="page-title">Dashboard</h1>

      <div className="stat-grid">
        <StatCard label="Étudiants"   value={stats?.totalStudents ?? '…'} color="#6366f1" />
        <StatCard label="CGPA Moyen"  value={stats ? stats.averageCgpa.toFixed(2) : '…'} color="#22c55e" />
        <StatCard label="Meilleur"    value={students[0]?.cgpa.toFixed(2) ?? '…'} color="#f59e0b" />
      </div>

      <div className="section">
        <div className="section-header">
          <h2>Classement par CGPA</h2>
          <button className="btn-link" onClick={() => onNavigate('Étudiants')}>Voir tout →</button>
        </div>
        {students.length === 0 ? (
          <p className="empty">Aucun étudiant. <button className="btn-link" onClick={() => onNavigate('Ajouter')}>En ajouter un</button></p>
        ) : (
          <div className="cgpa-bars">
            {students.map(s => <CgpaBar key={s.id} name={s.name} cgpa={s.cgpa} />)}
          </div>
        )}
      </div>
    </div>
  );
}
